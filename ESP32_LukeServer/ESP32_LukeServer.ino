#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"
#include "EEPROM.h"

#define EEPROM_SIZE 64

const char* ssid = "JunkerNet";
const char* password = "shanghai";

typedef struct
{
  char* contactName;
  char* phoneNumber;
  int amWindow;
  int pmWindow;
  boolean sendNow;
  int sendHour;
  boolean sentToday;
} Contact;

Contact Me = {"Nick", "+18035531371", 8, 24, false, 0, false};
Contact Grace = {"Grace", "+18032403328", 10, 20, false, 0, true};
Contact Nate = {"Nate", "+12543174621", 8, 24, false, 0, false};
Contact Luke = {"Luke", "18035920566", 8, 24, false, 0, true};
Contact Joong = {"Joong", "+18039173637", 8, 24, false, 0, false};

#define From "+18035923781"
#define TwilioUser "AC4bc970772f6accf39d8ff9a6e02b6a12"
#define TwilioPass "3f043a752680133b7321376feb254efd"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;
const double secInYear = 31536000;

boolean hasFucked = false;
boolean testBool = true;
boolean sendMessage = false;
int currentDay = 0;
int lastSent = 0;
double LukeAgeSec;
double LukeAgeYear;
char timeBuffer[30];
char msgBuf[80];
char postBuf[300];
char LukeAgeBuf[3];

String lukeStatus;
struct tm LukeDidItDate;

Contact contactList[] = {Me, Grace, Nate, Luke, Joong};
#define listLength 5
#define msgFreqOffset 1 //offset of 1 means every 1 day, 2 every other day, etc
boolean triggerSend = true;

void updateTime()
{
  time_t now;
  time(&now); //get current time; same as now = time();
  int randomHour;
  struct tm timeinfo;
  struct tm LukeBirthday;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo);
  LukeBirthday = *localtime(&now);   //instantiate LukeBirthday struct with correct timezone info
  LukeBirthday.tm_hour = 0; LukeBirthday.tm_min = 0; LukeBirthday.tm_sec = 0; //set struct to Luke's bday
  LukeBirthday.tm_mon = 8; LukeBirthday.tm_mday = 17; LukeBirthday.tm_year = 98;
  LukeAgeSec = difftime(now, mktime(&LukeBirthday)); //get difference in seconds
  LukeAgeYear = LukeAgeSec / secInYear;

  currentDay = timeinfo.tm_wday; //set time tickover to go on a day
  //tm_min, tm_sec
  if (currentDay != lastSent)
  {
    for (int i = 0; i < listLength; i++)
    {
      contactList[i].sentToday = false;
    }
  }
  if ((currentDay == (lastSent + msgFreqOffset) % 7) || (triggerSend == true)) //trigger send is for testing purposes
  { //7 days in a week, every x days, send text
    triggerSend = false;
    Serial.print("Current hour: ");
    Serial.println(timeinfo.tm_hour);
    for (int i = 0; i < listLength; i++)
    {
      int amWindow;
      int pmWindow;
      if (timeinfo.tm_hour > contactList[i].amWindow && timeinfo.tm_hour < contactList[i].pmWindow) //if hours have not been calculated, and it's past the window time,
      {                                                                                             //calculate for time->pmWindow, not a passed time ->pmWindow
        amWindow = timeinfo.tm_hour;
        pmWindow = contactList[i].pmWindow;
      } else
      {
        amWindow = contactList[i].amWindow;
        pmWindow = contactList[i].pmWindow;
      }
      contactList[i].sendHour = random(amWindow, pmWindow);
      Serial.print(contactList[i].contactName);
      Serial.print("'s sending hour: ");
      Serial.println(contactList[i].sendHour);
    }
    lastSent = currentDay;
  }
  for (int i = 0; i < listLength; i++)  //if it's in the right hour to send a message, flag to send
  {
    if (timeinfo.tm_hour == contactList[i].sendHour && contactList[i].sentToday == false)
    {
      contactList[i].sendNow = true;
    }
  }
  strftime(timeBuffer, 80, "%A, %B %d %Y at %I:%M %p", &timeinfo);
}

void formatMessage()
{

  //Now we format the message to be sent
  itoa((int)LukeAgeYear, LukeAgeBuf, 10);

  if (hasFucked == false)
  {
    lukeStatus = "Luke has not fucked.";
    strcpy(msgBuf, "As of ");
    strcat(msgBuf, timeBuffer);
    strcat(msgBuf, ", ");
    strcat(msgBuf, lukeStatus.c_str());
    strcat(msgBuf, " He is ");
    strcat(msgBuf, LukeAgeBuf);
    strcat(msgBuf, ".");
  } else
  {
    lukeStatus = "Luke fucked.";
    strcpy(msgBuf, "Holy shit. He finally did it. On ");
    strcat(msgBuf, timeBuffer);
    strcat(msgBuf, ", ");
    strcat(msgBuf, lukeStatus.c_str());
    strcat(msgBuf, " He was ");
    strcat(msgBuf, LukeAgeBuf);
    strcat(msgBuf, ".");
  }
}
void writeLukeDate()
{
  time_t now;
  time(&now);
  LukeDidItDate = *localtime(&now);
  LukeDidItDate.tm_hour = 3; LukeDidItDate.tm_min = 5; LukeDidItDate.tm_sec = 30;
  LukeDidItDate.tm_mon = 4; LukeDidItDate.tm_mday = 2; LukeDidItDate.tm_year = 105;
  int eeAddress = 0;
  //  EEPROM.put(eeAddress, hasFucked);
  //  eeAddress += sizeof(hasFucked);
  EEPROM.write(eeAddress, LukeDidItDate.tm_hour);
  EEPROM.write(1, LukeDidItDate.tm_min);
  EEPROM.write(2, LukeDidItDate.tm_sec);
  EEPROM.write(3, LukeDidItDate.tm_mon);
  EEPROM.write(4, LukeDidItDate.tm_mday);
  EEPROM.write(5, LukeDidItDate.tm_year);
  EEPROM.commit();
}
void updateLukeDate()
{

  Serial.println(EEPROM.length());
  /* for (int i = 0; i < EEPROM.length(); i++){
     EEPROM.write(i, 0);
     Serial.println("writing " + i);
    }
  */
  struct tm getData;
  Serial.print("Reading from EEPROM: ");
  int eeAddress = 0;
  // EEPROM.get(eeAddress, getData);
  /* if (hasFucked == true)
    {
     eeAddress = sizeof(hasFucked);
     EEPROM.get(eeAddress, LukeDidItDate);
    }
  */
  Serial.println("Read custom object from EEPROM: ");
  Serial.println(EEPROM.read(0));
  // Serial.println(hasFucked);
  // Serial.println(testBool);
  //Serial.println(&getData);
}
void setup () {
  Serial.begin(115200);
  Serial.println("start...");
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM"); delay(1000000);
  }

  randomSeed(4206969);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("Connecting..");
  }
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //writeLukeDate();
  // updateLukeDate();
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  currentDay = timeinfo.tm_wday;
  lastSent = currentDay;

}
void loop()
{

  if (WiFi.status() == WL_CONNECTED)
  { //Check WiFi connection status

    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("https://islukeavirgin.000webhostapp.com/");  //Specify request destination
    int httpCode = http.GET();        //Send the request

    if (httpCode > 0)
    { //Check the returning code

      String payload = http.getString();   //Get the request response payload
      if (payload.indexOf("YES") > 0)
      {
        hasFucked = false;
        Serial.println("Site response: YES");
      } else if (payload.indexOf("NO") > 0)
      {
        hasFucked = true;
        sendMessage =  true;
        Serial.println("Site response: NO");
      }
    } else {
      Serial.println("Error on checking Luke: " + httpCode);
    }
    http.end();   //Close connection

    updateTime();

    for (int i = 0; i < listLength; i++)
    {
      if (sendMessage == true || contactList[i].sendNow == true)
      {

        formatMessage();  //format the returned string
        //Connecting to Twilio servers
        http.begin("https://api.twilio.com/2010-04-01/Accounts/" TwilioUser "/Messages.json");
        http.setAuthorization(TwilioUser, TwilioPass);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        strcpy(postBuf, "From=" From);
        strcat(postBuf, "&To=");
        strcat(postBuf, contactList[i].phoneNumber);
        strcat(postBuf, "&Body=");
        strcat(postBuf, msgBuf);
        Serial.print("Sending to: ");
        Serial.println(contactList[i].contactName);

        int httpResponseCode = http.POST(postBuf); //send POST request with text data
        int numAttempts = 1;
        while (httpResponseCode <= 0 && numAttempts < 5)
        { //if message doesn't send, try 5 times to send
          Serial.print("Error on sending POST: ");
          Serial.println(httpResponseCode);
          Serial.print("Trying again, retry #");
          Serial.println(numAttempts);
          httpResponseCode = http.POST(postBuf);
          numAttempts++;
        }
        numAttempts = 0;
        String response = http.getString();  //Get the response to the request

        Serial.println(httpResponseCode);   //Print return code
        Serial.println(response);           //Print request answer

        http.end();
        sendMessage = false;
        contactList[i].sendNow = false;
        contactList[i].sentToday = true;
      }
    }
  }
  delay(30000);    //Send a request every 30 seconds
}
