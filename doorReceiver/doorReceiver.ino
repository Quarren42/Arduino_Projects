#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <VirtualWire.h>

int randNumber;
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin
#define RX_pin 8

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

boolean prevOpen = false;
boolean doorOpen = false;
boolean playSound = false;
boolean graham = false;
boolean luke = false;

typedef struct {
  String fileName;
  int percentChance;
} SoundClip;

SoundClip fillUp = {"fillUp.mp3", 5};
SoundClip jazz = {"jazz.mp3", 5};
SoundClip bathtub = {"bathtub.mp3", 5};
SoundClip gamecock = {"gamecock.mp3", 5};

SoundClip soundList[] = {fillUp, jazz, bathtub, gamecock};
int listLength = 4;

void setup()
{
  randomSeed(4206969);

  Serial.begin(9600);	// Debugging only

  if (! musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  musicPlayer.setVolume(20, 20);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);


  // Initialise the IO and ISR
  vw_set_rx_pin(RX_pin);
  vw_rx_start();
  vw_set_ptt_inverted(true);
  vw_setup(2000);	 // Bits per sec

}

void loop()
{

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  prevOpen = doorOpen;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    int i;

    digitalWrite(13, true); // Flash a light to show received good message
    // Message with a good checksum received, dump it.
    Serial.print("Got: ");

    for (i = 0; i < buflen; i++)
    {
      Serial.print(buf[i], DEC);
      Serial.print(" ");

      if (buf[i] == 71) { //71 is G
        graham = true;
      } else {
        graham = false;
      }
       if (buf[i] == 76) { //76 is L
        luke = true;
      } else {
        luke = false;
      }

      if (buf[i] == 89)
      {
        doorOpen = true;
      } else {
        doorOpen = false;
      }
    }
    Serial.println("");
    digitalWrite(13, false);
  }

  if (luke == true) {
    musicPlayer.playFullFile("f_l.mp3");
    luke = false;
  }
  if (graham == true) {
    musicPlayer.playFullFile("f_g.mp3");
    graham = false;
  }

  if (doorOpen == true && prevOpen == false) {
    playSound = true;
  }
  
  if (playSound == true) {
    int clipSelect = random(listLength);
    String songTitle = soundList[clipSelect].fileName;
    int randomPercent = random(101);
    if (randomPercent < soundList[clipSelect].percentChance)
    {
      musicPlayer.playFullFile(songTitle.c_str());
    } else {
      musicPlayer.playFullFile("Seinfeld.mp3");
    }
    playSound = false;
  }
}

