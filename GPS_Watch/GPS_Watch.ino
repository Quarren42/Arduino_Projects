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

#include <Adafruit_NeoPixel.h> //including all necessary libraries
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>


int timeOffset = TIME_OFFSET;

const int modeSelect = 4; //setting pin numbers
const int buttonLeft = 6;
const int buttonRight = 5;

Adafruit_NeoPixel ring = Adafruit_NeoPixel(12, 7, NEO_GRB + NEO_KHZ800); //initialize neopixel ring

SoftwareSerial mySerial(9, 8); //initialize softwareSerial on pins 8 and 9, because the hardware serial port is being used for testing
Adafruit_GPS GPS(&mySerial); //initialize the GPS

#define GPSECHO  false //no GPS Echo

boolean usingInterrupt = false;
void useInterrupt(boolean);

int currentHour; //initializing all variables
int currentMinute;
int currentSecond;

int oldCurrentHour;
int oldCurrentMinute;
int oldCurrentSecond;

int ringBrightness = 21; //ranges from 0 - 255, this is just the initial brightness
int brightnessLED = 0;

int mode = 0; //mode selection integer

int LEDoff = 0; //0 = LED's on, 1 = LED's off

int regionState = REGION;

uint32_t red = ring.Color(255, 0, 0); //stating 8 bit colour values for the neopixel ring to make chanigng colours easier later on
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



  pinMode(modeSelect, INPUT_PULLUP); //set inputs, buttons are pulled up to +5v to detect when they are grounded (switched on)
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);

  ring.begin(); //start neopixel ring
  ring.setBrightness(ringBrightness); //set brightness of ring
  ring.show(); //update neopixel ring
  Serial.begin(115200); //initialize serial port for testing
  Serial.println("Adafruit GPS library basic test!");

  GPS.begin(9600); //start GPS

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //some GPS config stuff

  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); //more GPS config stuff

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

uint32_t timer = millis(); //end of GPS config stuff

void loop()                 
{
  if(digitalRead(modeSelect) == LOW){ //if modeSelect is pressed, then
    mode = (mode + 1) % 4; //add one to the mode select integer, in modulo 4 (when the mode int = 4, it rolls back to 0)
    for(int i = 0; i < 12; i++){ //turn off all neopixels
      ring.setPixelColor(i, off); 
      ring.show(); //update ring
    }
    delay(420); //delay so only one button press is registered
  }
  switch (mode){ //switch statement for the mode selection
  case 0: //if case 0, start the clock mode
    modeClock();
    break;

  case 1: //if case 1, start the brightness config mode
    modeBrightness();
    break;

  case 2: //if case 2, start the flashlight mode
    modeFlashlight();
    break;

  case 3: //if  case 3, start the TV-B-GONE mode
    modeTVbGone();
    break;
  }
}

void modeFlashlight(){ //function for the flashlight mode
  screenOff(); //check to see if left button is pressed to turn off the screen
  for (int i = 0; i < 12; i ++){ //set all 12 neopixels to the designated flashlight colour
    ring.setPixelColor(i, FLASH_COLOUR);
  } 
  ring.show(); //update neopixel ring
}

void modeBrightness(){ //function for the brightness select mode
  screenOff(); //check to see if the left button has been pressed
  if (digitalRead(buttonRight) == LOW){ //if the right button is pressed
    ringBrightness = (ringBrightness + 21) % 253; //set the ring brightness 21 brighter, in modulo 253 (instead of 255, because math)
    brightnessLED = (brightnessLED + 1) % 12; //turn on the following LED in the ring, in modulo 12
    if (brightnessLED == 0){ //once all LEDs are lit, the next button press resets the brightness back to 21
      for (int i = 1; i < 12; i ++){ //turn off all neopixels
        ring.setPixelColor(i, off);
      }
    }
    delay(200);
  }
  ring.setBrightness(ringBrightness); //set the ring brightness 
  ring.setPixelColor(brightnessLED, red); //change the colour to red
  ring.show(); //update the neopixel ring
}

void modeClock(){ //function for the clock mode
  screenOff(); //check for the left button press to turn off the screen
  if (! usingInterrupt) { //some GPS stuff
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

  oldCurrentHour = currentHour; //set the oldHour/Minute/Second values before we update to the new time
  oldCurrentMinute = currentMinute;
  oldCurrentSecond = currentSecond;

  currentHour = (GPS.hour + timeOffset + 1) % 12; //get the current hour by adding our timeOffset (time zones) to the current GMT time (GPS.hour)
  currentMinute = GPS.minute / 5; //divide by 5 to change after 5 minutes, as there are 12 LEDs. We also add 1 above to correct for the neopixels being off by one
  currentSecond = GPS.seconds / 5; //same^

  if ((currentHour != oldCurrentHour) && ((currentHour - 1) != currentMinute || currentSecond)){ //set pixel behind new one off, to remove duplicates (don't want the whole ring to fill up, just one LED at a time)
    ring.setPixelColor(((currentHour + 11) % 12), off); //if the time has changed, and the Hour/Minute/Second LEDs don't overlap, turn the previous one off
  }

  if ((currentMinute != oldCurrentMinute) && ((currentMinute - 1) != currentHour || currentSecond)){ //same as above, but for the minutes
    ring.setPixelColor(((currentMinute + 11) % 12), off);
  }

  if ((currentSecond != oldCurrentSecond) && ((currentSecond - 1) != currentHour || currentMinute)) { //same as above, but for the seconds
    ring.setPixelColor(((currentSecond + 11) % 12), off);
  }
  
  ring.setPixelColor((currentHour), red); //set the hour LED red
  ring.setPixelColor(currentMinute, green); //set the minute LED green
  ring.setPixelColor(currentSecond, blue); //set the second LED blue


  if (currentHour == currentMinute){ //if the red hour LED and the green minute LED overlap, change the colour to yellow (mix the colours)
    ring.setPixelColor(currentHour, yellow);
  }
  if (currentMinute == currentSecond){ //mix the overlapping colours
    ring.setPixelColor(currentMinute, cyan);
  }
  if (currentSecond == currentHour){ //mix some more
    ring.setPixelColor(currentSecond, purple);
  }
  if ((currentSecond == currentHour) && (currentSecond == currentMinute)){ //man I sure do love mixing 
    ring.setPixelColor(currentSecond, white);
  }

  ring.show(); //update the neopixel ring
  delay (100); //delay to free up some time on the Arduino

  if (timer > millis())  timer = millis(); //just outputting all time info to the Serial port for debugging
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

    if (GPS.fix) { //output the GPS fixation data to the serial port for debugging
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
void modeTVbGone(){ //function for the TV-B-GONE mode
  
  if (digitalRead(buttonLeft) == LOW){ //if the left button is pressed, then change the region (different IR codes for different places)
    regionState = (regionState + 1) % 2; //rollover the value at 2
    delay(200); //delay to prevent multiple button presses being registered
  }
  
  if (regionState == 0){ //if the region is 0, set it to North America
    region = NA;
    num_codes = num_NAcodes; //set the IR codes to the NA ones
    ring.setPixelColor(5, blue); //set the LEDs to red, white and blue to signify 'murica
    ring.setPixelColor(6, white);
    ring.setPixelColor(7, red);
  }

  else {
    region = EU; //else set the region to Europe
    num_codes = num_EUcodes; //set the IR codes to the EU ones
    ring.setPixelColor(5, blue); //set the LEDs to European colours
    ring.setPixelColor(6, yellow); 
    ring.setPixelColor(7, blue); //I may have had to ask google what the european colours were
  }
  ring.show(); //update neopixel ring
  if (digitalRead(buttonRight) == LOW){ //if the right button is pressed, start transmitting the IR codes
    sendAllCodes(); //wreck havoc :D :3 :D :3 :D :3
  }
}

void screenOff(){ //function for turning the screen off to save power
  if (digitalRead(buttonLeft) == LOW){ //if the left button is pressed
    LEDoff = (LEDoff + 1) % 2; //rollover the LEDoff value
    delay(200);
  }

  while (LEDoff == 1){ //WHILE the LEDoff value is 1
    for (int i = 0; i < 12; i ++){ //constantly turn off all LEDs
      ring.setPixelColor(i, off);
      ring.show(); //update the neopixel ring
      if (digitalRead(buttonLeft) == LOW){ //within the WHILE loop, check if the button is pressed again
        LEDoff = (LEDoff + 1) % 2; //if so, rollover the LEDoff value, to exit the LEDoff mode
        delay(200); //delay to prevent duplicate button presses
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

























