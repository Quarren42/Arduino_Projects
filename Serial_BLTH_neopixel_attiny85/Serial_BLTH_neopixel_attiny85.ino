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
int incomingByte4 = 0; //lower bound value of which neopixel to light
int incomingByte5 = 0; //upper bound value of which neopixel to light
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

  pinMode(PIN, OUTPUT);
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);
  mySerial.begin(9600);
  ring.show(); //do a ring.show at the beginning to clear the neopixels
}

void loop() {
  char buffer[] = {' ', ' ', ' ', ' ', ' ', ' ', ' '}; 
  while (!mySerial.available()); //if not available, read
  mySerial.readBytesUntil('\n', buffer, 7); //read what number is coming through (1,2,3, etc)
  int incomingByte = atoi(buffer); //converts ascii to int
  mySerial.println(incomingByte); //print to serial for debugging

  switch (incomingByte){ //switch for first value

  case 1: //case 1 handles the color setting
    {
      char buffer2[] = {' ',' ',' ',' ',' ',' ',' '}; //read value for type of color coming through
      while (!mySerial.available()); //same as above
      mySerial.readBytesUntil('\n', buffer2, 7); //read until \n
      incomingByte2 = atoi(buffer2); //again, ascii to int
    }

    switch (incomingByte2){ //switch for second value, which is the type of color

    case 0: //lights up all pixels for the specified color
      for (int i = 0; i < NumofPixels; i++){
        ring.setPixelColor(i, red);
        ring.show();
      }
      break;

    case 1: 
      for (int i = 0; i < NumofPixels; i++){
        ring.setPixelColor(i, green);
        ring.show();
      }
      break;

    case 2:
      for (int i = 0; i < NumofPixels; i++){
        ring.setPixelColor(i, blue);
        ring.show();
      }
      break;

    case 3:
      for (int i = 0; i < NumofPixels; i++){
        ring.setPixelColor(i, yellow);
        ring.show();
      }
      break;

    case 4:
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
      char buffer3[] = {' ', ' ', ' ', ' '}; 
      while (!mySerial.available()); 
      mySerial.readBytesUntil('\n', buffer3, 4); //read until \n
      incomingByte3 = atoi(buffer3); //ascii to int
      ring.setBrightness(incomingByte3); //set brightness to whatever value the third incoming value was
      ring.show(); //update ring
    }
    break; //end of brightness mode
 
    
    case 3: //case 3 handles which LEDs are lit up
    {
        char buffer4[] = {' ', ' ', ' ', ' '}; //set the buffers 
        char buffer5[] = {' ', ' ', ' ', ' '};
        char buffer6[] = {' ', ' ', ' ', ' '};
        
          while (!mySerial.available()); 
      mySerial.readBytesUntil('\n', buffer4, 4); //read for the lower bound value of what pixel to light up
       incomingByte4 = atoi(buffer4);
       
      while(!mySerial.available()); 
      mySerial.readBytesUntil('\n', buffer5, 4); //read for the upper bound value of what pixel to light up
      incomingByte5 = atoi(buffer5);
        
        while (!mySerial.available());
        mySerial.readBytesUntil('\n', buffer6, 4); //read for what color to change to
        incomingByte6 = atoi(buffer6);
        
        switch (incomingByte6){ //switch for second value, which is the type of color
      
     case 0: //lights up all pixels for the specified color
      for (int i = incomingByte4; i <= incomingByte5; i++){ //make int i the values between the lower and upper bound numbers for each color
        ring.setPixelColor(i, red);
        ring.show();
      }
      break;

    case 1: 
      for (int i = incomingByte4; i <= incomingByte5; i++){ //same as case 0
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
















