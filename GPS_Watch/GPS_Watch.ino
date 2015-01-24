/* ADJUST TIME ZONE, GPS COORDINATES, TV-B-GONE REGION, & FLASHLIGHT COLOUR BELOW  */

#define TIME_OFFSET            19

#define GEO_LAT                44.998531
#define GEO_LON               -93.230322 

#define REGION                 0 

#define FLASH_COLOUR          white

#define TV_COLOUR             yellow

/* To adjust time zone, instead of adding / subtracting your time zone from GMT, add/subtract it from 24 */
/* for example: Eastern Time is GMT-5, so I will subtract 5 from 24, which equals 19, then enter that directly after the equals sign above */
/* region 0 is for NA, region 1 is EU/etc */


/* ALL OF THIS IS FOR THE TV-B-GONE */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
#include "main.h"
#include <avr/sleep.h>

void xmitCodeElement(uint16_t ontime, uint16_t offtime, uint8_t PWM_code );
void quickflashLEDx( uint8_t x );
void delay_ten_us(uint16_t us);
void quickflashLED( void );
uint8_t read_bits(uint8_t count);

#define putstring_nl(s) Serial.println(s)
#define putstring(s) Serial.print(s)
#define putnum_ud(n) Serial.print(n, DEC)
#define putnum_uh(n) Serial.print(n, HEX)

extern PGM_P *NApowerCodes[] PROGMEM;
extern PGM_P *EUpowerCodes[] PROGMEM;
extern uint8_t num_NAcodes, num_EUcodes;

void xmitCodeElement(uint16_t ontime, uint16_t offtime, uint8_t PWM_code )
{
  TCNT2 = 0;
  if(PWM_code) {
    pinMode(IRLED, OUTPUT);
    // Fast PWM, setting top limit, divide by 8
    // Output to pin 3
    TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(WGM22) | _BV(CS21);
  }
  else {
    // However some codes dont use PWM in which case we just turn the IR
    // LED on for the period of time.
    digitalWrite(IRLED, HIGH);
  }

  // Now we wait, allowing the PWM hardware to pulse out the carrier
  // frequency for the specified 'on' time
  delay_ten_us(ontime);

  // Now we have to turn it off so disable the PWM output
  TCCR2A = 0;
  TCCR2B = 0;
  // And make sure that the IR LED is off too (since the PWM may have
  // been stopped while the LED is on!)
  digitalWrite(IRLED, LOW);

  // Now we wait for the specified 'off' time
  delay_ten_us(offtime);
}
uint8_t bitsleft_r = 0;
uint8_t bits_r=0;
PGM_P code_ptr;

// we cant read more than 8 bits at a time so dont try!
uint8_t read_bits(uint8_t count)
{
  uint8_t i;
  uint8_t tmp=0;

  // we need to read back count bytes
  for (i=0; i<count; i++) {
    // check if the 8-bit buffer we have has run out
    if (bitsleft_r == 0) {
      // in which case we read a new byte in
      bits_r = pgm_read_byte(code_ptr++);
      // and reset the buffer size (8 bites in a byte)
      bitsleft_r = 8;
    }
    // remove one bit
    bitsleft_r--;
    // and shift it off of the end of 'bits_r'
    tmp |= (((bits_r >> (bitsleft_r)) & 1) << (count-1-i));
  }
  // return the selected bits in the LSB part of tmp
  return tmp;
}

uint16_t ontime, offtime;
uint8_t i,num_codes, Loop;
uint8_t region;
uint8_t startOver;

#define FALSE 0
#define TRUE 1
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* END OF TV-B-GONE CODE */

#include <Adafruit_NeoPixel.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>


int timeOffset = TIME_OFFSET;

const int modeSelect = 4;
const int buttonLeft = 6;
const int buttonRight = 5;

Adafruit_NeoPixel ring = Adafruit_NeoPixel(12, 7, NEO_GRB + NEO_KHZ800);

SoftwareSerial mySerial(9, 8); 
Adafruit_GPS GPS(&mySerial); 

#define GPSECHO  false

boolean usingInterrupt = false;
void useInterrupt(boolean);

int currentHour;
int currentMinute;
int currentSecond;

int oldCurrentHour;
int oldCurrentMinute;
int oldCurrentSecond;

int ringBrightness = 21;
int brightnessLED = 0;

int mode = 0;

int LEDoff = 0; //0 = LED's on, 1 = LED's off

int regionState = REGION;

uint32_t red = ring.Color(255, 0, 0);
uint32_t green = ring.Color(0, 255, 0);
uint32_t blue = ring.Color(0, 0, 255);
uint32_t yellow = ring.Color(255, 255, 0);
uint32_t cyan = ring.Color(0, 255, 255);
uint32_t purple = ring.Color(255, 0, 255);
uint32_t white = ring.Color(255, 255, 255);

uint32_t off = ring.Color(0, 0, 0);

void setup()  
{
  /* MORE TV-B-GONE CODE */
  /* ------------------------------------------ */
  /* ------------------------------------------ */
  /* ------------------------------------------ */
  /* ------------------------------------------ */
  /* ------------------------------------------ */
  TCCR2A = 0;
  TCCR2B = 0;

  digitalWrite(LED, LOW);
  digitalWrite(IRLED, LOW);
  pinMode(LED, OUTPUT);
  pinMode(IRLED, OUTPUT);

  delay_ten_us(5000);            // Let everything settle for a bit

  // Indicate how big our database is
  DEBUGP(putstring("\n\rNA Codesize: ");
  putnum_ud(num_NAcodes);
  );
  DEBUGP(putstring("\n\rEU Codesize: ");
  putnum_ud(num_EUcodes);
  );
  /* ------------------------------------------ */
  /* ------------------------------------------ */
  /* ------------------------------------------ */
  /* ------------------------------------------ */
  /* ------------------------------------------ */
  /* END TV-B-GONE CODE */



  pinMode(modeSelect, INPUT_PULLUP);
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);

  ring.begin();
  ring.setBrightness(ringBrightness);
  ring.show();
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");

  GPS.begin(9600);

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  useInterrupt(true);

  delay(1000);

  mySerial.println(PMTK_Q_RELEASE);
}

SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();

#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  

#endif
}

void useInterrupt(boolean v) {
  if (v) {

    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } 
  else {

    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();
void loop()                 
{
  if(digitalRead(modeSelect) == LOW){
    mode = (mode + 1) % 4;
    for(int i = 0; i < 12; i++){
      ring.setPixelColor(i, off);
      ring.show();
    }
    delay(420);
  }
  switch (mode){
  case 0:
    modeClock();
    break;

  case 1:
    modeBrightness();
    break;

  case 2:
    modeFlashlight();
    break;

  case 3:
    modeTVbGone();
    break;
  }
}
void modeFlashlight(){
  screenOff();
  for (int i = 0; i < 12; i ++){
    ring.setPixelColor(i, FLASH_COLOUR);
  } 
  ring.show();
}

void modeBrightness(){
  screenOff();
  if (digitalRead(buttonRight) == LOW){
    ringBrightness = (ringBrightness + 21) % 253;
    brightnessLED = (brightnessLED + 1) % 12;
    if (brightnessLED == 0){
      for (int i = 1; i < 12; i ++){
        ring.setPixelColor(i, off);
      }
    }
    delay(200);
  }
  ring.setBrightness(ringBrightness);
  ring.setPixelColor(brightnessLED, red);
  ring.show();
}

void modeClock(){
  screenOff();
  if (! usingInterrupt) {
    char c = GPS.read();
    if (GPSECHO)
      if (c) Serial.print(c);
  }
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  oldCurrentHour = currentHour;
  oldCurrentMinute = currentMinute;
  oldCurrentSecond = currentSecond;

  currentHour = (GPS.hour + timeOffset + 1) % 12; //set which pixel is on for hour/minute/second, chances 24 hour to 12 hour
  currentMinute = GPS.minute / 5;
  currentSecond = GPS.seconds / 5; 

  if ((currentHour != oldCurrentHour) && ((currentHour - 1) != currentMinute || currentSecond)){ //set pixel behind new one off, to remove dupes
    ring.setPixelColor(((currentHour + 11) % 12), off);
  }

  if ((currentMinute != oldCurrentMinute) && ((currentMinute - 1) != currentHour || currentSecond)){ //same^
    ring.setPixelColor(((currentMinute + 11) % 12), off);
  }

  if ((currentSecond != oldCurrentSecond) && ((currentSecond - 1) != currentHour || currentMinute)) { //same^
    ring.setPixelColor(((currentSecond + 11) % 12), off);
  }
  
  ring.setPixelColor((currentHour), red);
  ring.setPixelColor(currentMinute, green);
  ring.setPixelColor(currentSecond, blue);


  if (currentHour == currentMinute){
    ring.setPixelColor(currentHour, yellow);
  }
  if (currentMinute == currentSecond){
    ring.setPixelColor(currentMinute, cyan);
  }
  if (currentSecond == currentHour){
    ring.setPixelColor(currentSecond, purple);
  }
  if ((currentSecond == currentHour) && (currentSecond == currentMinute)){
    ring.setPixelColor(currentSecond, white);
  }

  ring.show();
  delay (100);

  if (timer > millis())  timer = millis();
  if (millis() - timer > 2000) { 
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); 
    Serial.print(':');
    Serial.print(GPS.minute, DEC); 
    Serial.print(':');
    Serial.print(GPS.seconds, DEC); 
    Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); 
    Serial.print('/');
    Serial.print(GPS.month, DEC); 
    Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); 
    Serial.print((int)GPS.fix);
    Serial.print(" quality: "); 
    Serial.println((int)GPS.fixquality); 
    Serial.println(currentHour);
    Serial.println(currentMinute);
    Serial.println(currentSecond);

    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); 
      Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); 
      Serial.println(GPS.lon);

      Serial.print("Speed (knots): "); 
      Serial.println(GPS.speed);
      Serial.print("Angle: "); 
      Serial.println(GPS.angle);
      Serial.print("Altitude: "); 
      Serial.println(GPS.altitude);
      Serial.print("Satellites: "); 
      Serial.println((int)GPS.satellites);

    }
  }
}
void modeTVbGone(){
  
  if (digitalRead(buttonLeft) == LOW){
    regionState = (regionState + 1) % 2;
    delay(200);
  }
  
  if (regionState == 0){
    region = NA;
    num_codes = num_NAcodes;
    ring.setPixelColor(5, blue);
    ring.setPixelColor(6, white);
    ring.setPixelColor(7, red);
  }

  else {
    region = EU;
    num_codes = num_EUcodes;
    ring.setPixelColor(5, blue);
    ring.setPixelColor(6, yellow);
    ring.setPixelColor(7, blue);
  }
  ring.show();
  if (digitalRead(buttonRight) == LOW){
    sendAllCodes();
  }
}

void screenOff(){
  if (digitalRead(buttonLeft) == LOW){
    LEDoff = (LEDoff + 1) % 2;
    delay(200);
  }

  while (LEDoff == 1){
    for (int i = 0; i < 12; i ++){
      ring.setPixelColor(i, off);
      ring.show();
      if (digitalRead(buttonLeft) == LOW){
        LEDoff = (LEDoff + 1) % 2;
        delay(200);
      }
    }
  }   
}

/* MORE TV-B-GONE CODE */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
void sendAllCodes() {
Start_transmission:
  // startOver will become TRUE if the user pushes the Trigger button while transmitting the sequence of all codes
  startOver = FALSE;

  // for every POWER code in our collection
  for (i=0 ; i < num_codes; i++) {
    PGM_P data_ptr;

    // print out the code # we are about to transmit
    DEBUGP(putstring("\n\r\n\rCode #: ");
    putnum_ud(i));

    // point to next POWER code, from the right database
    if (region == NA) {
      data_ptr = (PGM_P)pgm_read_word(NApowerCodes+i);
    }
    else {
      data_ptr = (PGM_P)pgm_read_word(EUpowerCodes+i);
    }

    // print out the address in ROM memory we're reading
    DEBUGP(putstring("\n\rAddr: ");
    putnum_uh((uint16_t)data_ptr));

    // Read the carrier frequency from the first byte of code structure
    const uint8_t freq = pgm_read_byte(data_ptr++);
    // set OCR for Timer1 to output this POWER code's carrier frequency
    OCR2A = freq;
    OCR2B = freq / 3; // 33% duty cycle

    // Print out the frequency of the carrier and the PWM settings
    DEBUGP(putstring("\n\rOCR1: ");
    putnum_ud(freq);
    );
    DEBUGP(uint16_t x = (freq+1) * 2;
    putstring("\n\rFreq: ");
    putnum_ud(F_CPU/x);
    );

    // Get the number of pairs, the second byte from the code struct
    const uint8_t numpairs = pgm_read_byte(data_ptr++);
    DEBUGP(putstring("\n\rOn/off pairs: ");
    putnum_ud(numpairs));

    // Get the number of bits we use to index into the timer table
    // This is the third byte of the structure
    const uint8_t bitcompression = pgm_read_byte(data_ptr++);
    DEBUGP(putstring("\n\rCompression: ");
    putnum_ud(bitcompression);
    putstring("\n\r"));

    // Get pointer (address in memory) to pulse-times table
    // The address is 16-bits (2 byte, 1 word)
    PGM_P time_ptr = (PGM_P)pgm_read_word(data_ptr);
    data_ptr+=2;
    code_ptr = (PGM_P)pgm_read_word(data_ptr);

    // Transmit all codeElements for this POWER code
    // (a codeElement is an onTime and an offTime)
    // transmitting onTime means pulsing the IR emitters at the carrier
    // frequency for the length of time specified in onTime
    // transmitting offTime means no output from the IR emitters for the
    // length of time specified in offTime

#if 0

    // print out all of the pulse pairs
    for (uint8_t k=0; k<numpairs; k++) {
      uint8_t ti;
      ti = (read_bits(bitcompression)) * 4;
      // read the onTime and offTime from the program memory
      ontime = pgm_read_word(time_ptr+ti);
      offtime = pgm_read_word(time_ptr+ti+2);
      DEBUGP(putstring("\n\rti = ");
      putnum_ud(ti>>2);
      putstring("\tPair = ");
      putnum_ud(ontime));
      DEBUGP(putstring("\t");
      putnum_ud(offtime));
    }
    continue;
#endif

    // For EACH pair in this code....
    cli();
    for (uint8_t k=0; k<numpairs; k++) {
      uint16_t ti;

      // Read the next 'n' bits as indicated by the compression variable
      // The multiply by 4 because there are 2 timing numbers per pair
      // and each timing number is one word long, so 4 bytes total!
      ti = (read_bits(bitcompression)) * 4;

      // read the onTime and offTime from the program memory
      ontime = pgm_read_word(time_ptr+ti);  // read word 1 - ontime
      offtime = pgm_read_word(time_ptr+ti+2);  // read word 2 - offtime
      // transmit this codeElement (ontime and offtime)
      xmitCodeElement(ontime, offtime, (freq!=0));
    }
    sei();

    //Flush remaining bits, so that next code starts
    //with a fresh set of 8 bits.
    bitsleft_r=0;

    // delay 205 milliseconds before transmitting next POWER code
    delay_ten_us(20500);

    // visible indication that a code has been output.
    quickflashLED();

    // if user is pushing Trigger button, stop transmission
    if (digitalRead(buttonRight) == LOW){
      startOver = TRUE;
      break;
    } 
  }

  if (startOver) goto Start_transmission;
  while (Loop == 1);

  // flash the visible LED on PB0  8 times to indicate that we're done
  delay_ten_us(65500); // wait maxtime
  delay_ten_us(65500); // wait maxtime
  for (int i = 0; i < 8; i ++){
    ring.setPixelColor(0, TV_COLOUR);
    ring.setPixelColor(1, TV_COLOUR);
    ring.setPixelColor(2, TV_COLOUR);
    ring.setPixelColor(3, TV_COLOUR);
    ring.setPixelColor(4, TV_COLOUR);
    ring.setPixelColor(8, TV_COLOUR);
    ring.setPixelColor(9, TV_COLOUR);
    ring.setPixelColor(10, TV_COLOUR);
    ring.setPixelColor(11, TV_COLOUR);
    delay(200);
    ring.show();
    ring.setPixelColor(0, off);
    ring.setPixelColor(1, off);
    ring.setPixelColor(2, off);
    ring.setPixelColor(3, off);
    ring.setPixelColor(4, off);
    ring.setPixelColor(8, off);
    ring.setPixelColor(9, off);
    ring.setPixelColor(10, off);
    ring.setPixelColor(11, off);
    delay(100);
    ring.show();
  }
}
void delay_ten_us(uint16_t us) {
  uint8_t timer;
  while (us != 0) {
    // for 8MHz we want to delay 80 cycles per 10 microseconds
    // this code is tweaked to give about that amount.
    for (timer=0; timer <= DELAY_CNT; timer++) {
      NOP;
      NOP;
    }
    NOP;
    us--;
  }
}
// This function quickly pulses the visible LED (connected to PB0, pin 5)
// This will indicate to the user that a code is being transmitted
void quickflashLED( void ) {

  ring.setPixelColor(0, TV_COLOUR);
  ring.show();
  delay_ten_us(3000);   // 30 millisec delay
  ring.setPixelColor(0, off);
  ring.show();
}

/* void sleepNow()
 {
 set_sleep_mode(TRIGGER);                    // sleep mode is set here
 
 sleep_enable();                             // enables the sleep bit in the mcucr register
 
 attachInterrupt(0, wakeUpNow, LOW);         // use interrupt 0 (pin 2) and run function
 // wakeUpNow when pin 2 gets LOW
 
 sleep_mode();                               // here the device is actually put to sleep!!
 // THE PROGRAM CONTINUES FROM HERE ON WAKE
 
 sleep_disable();                            // first thing after waking, disable sleep
 
 detachInterrupt(0);                         // disables int 0 as the wakeupnow code will
 // not be executed during normal runtime
 }
 
 */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* ------------------------------------------ */
/* END TV-B-GONE CODE */

























