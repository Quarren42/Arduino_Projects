#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define NumofPixels 30 //number of neopixels
#define PIN 0 //data pin for neopixels

Adafruit_NeoPixel ring = Adafruit_NeoPixel(NumofPixels, PIN); //create the neopixel object

const int rx=3; //set up pins for software serial
const int tx=4;

SoftwareSerial mySerial(rx, tx); //declare what pins are for softwareserial

int incomingByte = 0; //mode selection
int incomingByte2 = 0; //type of color (red, blue, etc)
int incomingByte3 = 0; //brightness of ring
int incomingByte4 = 0; //lower bound value of which neopixel to light (for specific neopixel lighting mode)
int incomingByte5 = 0; //upper bound value of which neopixel to light (for specific neopixel lighting mode)
int incomingByte6 = 0; //type of color
int brightness = 0; 

uint32_t red = ring.Color(255, 0, 0);  //making things easier by naming the specific colors
uint32_t green = ring.Color(0, 255, 0);
uint32_t blue = ring.Color(0, 0, 255);
uint32_t yellow = ring.Color(255, 255, 0);
uint32_t cyan = ring.Color(0, 255, 255);
uint32_t purple = ring.Color(255, 0, 255);
uint32_t white = ring.Color(255, 255, 255);

uint32_t off = ring.Color(0, 0, 0);

void setup() {

  pinMode(PIN, OUTPUT); //data pin for neopixels
  pinMode(rx, INPUT);  //setting up pins for the softwareSerial
  pinMode(tx, OUTPUT); //softwareSerial is needed for the Bluetooth SPP link
  mySerial.begin(9600); //initialize serial port
  ring.show(); //do a ring.show at the beginning to clear the neopixels

/*  for (int i = 0; i < NumofPixels; i++){ //turn on LEDs white when first started up
    ring.setPixelColor(i, white);
    ring.show(); 
  }
  
  */
  
  christmasTheme();
}

void loop() {
  char buffer[] = {
    ' ', ' ', ' ', ' ', ' ', ' ', ' '  }; //set a buffer for the data being sent to the attiny
  while (!mySerial.available()); //will only continue past this line if serial data is available; allows the data to fully load into the buffer
  mySerial.readBytesUntil('\n', buffer, 7); //read what number is coming through (1,2,3, etc)
  int incomingByte = atoi(buffer); //converts ascii buffer to integers
  mySerial.println(incomingByte); //print to serial for debugging

  switch (incomingByte){ //switch for first value, determines the colour

  case 1: //case 1 handles the color setting
    {
      char buffer2[] = {
        ' ',' ',' ',' ',' ',' ',' '      }; //read value for type of color coming through
      while (!mySerial.available()); //will only continue if serial data is available
      mySerial.readBytesUntil('\n', buffer2, 7); //read until \n to signify the end of the data string
      incomingByte2 = atoi(buffer2); //if 1 is sent, the colour setting is selected - the specific colour data (incomingByte2) is then sent
      //into another switch statement
    }

    switch (incomingByte2){ //switch for second value, which is the type of color

    case 0: //lights up all pixels for the specified color
      for (int i = 0; i < NumofPixels; i++){ //light up all neopixels red
        ring.setPixelColor(i, red);
        ring.show(); //update the strip (called a ring, but it's a strip)
      }
      break;

    case 1: //same as above,  but for green
      for (int i = 0; i < NumofPixels; i++){
        ring.setPixelColor(i, green);
        ring.show();
      }
      break;

    case 2: //all the same
      for (int i = 0; i < NumofPixels; i++){
        ring.setPixelColor(i, blue);
        ring.show();
      }
      break;

    case 3: //still the same...
      for (int i = 0; i < NumofPixels; i++){
        ring.setPixelColor(i, yellow);
        ring.show();
      }
      break;

    case 4: //well this is repetitive 
      for (int i = 0; i < NumofPixels; i++){
        ring.setPixelColor(i, cyan);
        ring.show();
      }
      break;

    case 5:
      for (int i = 0; i < NumofPixels; i++){
        ring.setPixelColor(i, purple);
        ring.show();
      }
      break;

    case 6:
      for (int i = 0; i < NumofPixels; i++){
        ring.setPixelColor(i, white);
        ring.show();
      }
      break;

    case 7:
      for (int i = 0; i < NumofPixels; i++){
        ring.setPixelColor(i, off);
        ring.show();
      }
      break;
    }
    break; //end of the color mode


  case 2: //case 2 handles the brightness
    {
      char buffer3[] = {
        ' ', ' ', ' ', ' '      }; //buffer for the incoming data
      while (!mySerial.available()); //will only continue past this point if serial data is available
      mySerial.readBytesUntil('\n', buffer3, 4); //read until \n
      incomingByte3 = atoi(buffer3); //ascii buffer to int
      ring.setBrightness(incomingByte3); //set brightness to whatever value the third incoming value was
      ring.show(); //update ring
    }
    break; //end of brightness mode


  case 3: //case 3 handles which LEDs are lit up
    {
      char buffer4[] = {
        ' ', ' ', ' ', ' '      }; //set the buffers 
      char buffer5[] = {
        ' ', ' ', ' ', ' '      };
      char buffer6[] = {
        ' ', ' ', ' ', ' '      };

      while (!mySerial.available()); //waits for the next data to be available
      mySerial.readBytesUntil('\n', buffer4, 4); //read for the lower bound value of what pixel to light up
      incomingByte4 = atoi(buffer4); //convert the buffer to an integer

      while(!mySerial.available()); //waits for more data to be available
      mySerial.readBytesUntil('\n', buffer5, 4); //read for the upper bound value of what pixel to light up
      incomingByte5 = atoi(buffer5); //ascii buffer to integer

      while (!mySerial.available()); //same as above
      mySerial.readBytesUntil('\n', buffer6, 4); //read for what color to change to
      incomingByte6 = atoi(buffer6); 

      switch (incomingByte6){ //switch for colour setting of the specific neopixel colouring mode

      case 0: //lights up all pixels for the specified color, which is red
        for (int i = incomingByte4; i <= incomingByte5; i++){ //colours in the selected (lower bound to upper bound) neopixels
          ring.setPixelColor(i, red); //set the colours to red
          ring.show(); 
        }
        break;

      case 1: 
        for (int i = incomingByte4; i <= incomingByte5; i++){ //same as above
          ring.setPixelColor(i, green);
          ring.show();
        }
        break;

      case 2:
        for (int i = incomingByte4; i <= incomingByte5; i++){
          ring.setPixelColor(i, blue);
          ring.show();
        }
        break;

      case 3:
        for (int i = incomingByte4; i <= incomingByte5; i++){
          ring.setPixelColor(i, yellow);
          ring.show();
        }
        break;

      case 4:
        for (int i = incomingByte4; i <= incomingByte5; i++){
          ring.setPixelColor(i, cyan);
          ring.show();
        }
        break;

      case 5:
        for (int i = incomingByte4; i <= incomingByte5; i++){
          ring.setPixelColor(i, purple);
          ring.show();
        }
        break;

      case 6:
        for (int i = incomingByte4; i <= incomingByte5; i++){
          ring.setPixelColor(i, white);
          ring.show();
        }
        break;

      case 7:
        for (int i = incomingByte4; i <= incomingByte5; i++){
          ring.setPixelColor(i, off);
          ring.show();
        }
        break;

      } //end of switch 6
    } //end of case 3
    break;

  } //end of primary switch statement
} //end of sketch

void christmasTheme(){
  for (int i = 0; i <= 15; i++){
    ring.setPixelColor(i, red);
    ring.show();
    delay(50);
  }

  for (int i = 16; i <= 29; i++){
    ring.setPixelColor(i, green);
    ring.show();
    delay(50);
  }
} 















