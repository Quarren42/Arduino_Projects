#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define NumofPixels 45
#define PIN 5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NumofPixels, PIN, NEO_GRB + NEO_KHZ800);

const int rx = 6; //software serial pins
const int tx = 7;
const char DELIM = 'A';

SoftwareSerial mySerial(rx,tx);

uint32_t red = strip.Color(255, 0, 0);  //making things easier by naming the specific colors
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t yellow = strip.Color(255, 180, 0);
uint32_t cyan = strip.Color(0, 255, 255);
uint32_t purple = strip.Color(255, 0, 255);
uint32_t white = strip.Color(255, 255, 255);

uint32_t off = strip.Color(0, 0, 0);

int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int brightnessValue = 255;

void setup()
{
  mySerial.begin(9600);
  mySerial.setTimeout(200);
  strip.begin();
  strip.show();
}
void setColor(int r, int g, int b)
{
  uint32_t currColor = strip.Color(r, g, b);
  for (int i = 0; i < NumofPixels; i++)
  {
    strip.setPixelColor(i, currColor); 
  }
}
void loop()
{
  //String input;
  char buffer[] ={' ',' ',' ',' ',' '};
  char fIndex;
  while (!mySerial.available());
  mySerial.readBytesUntil(DELIM, buffer, 5);
  fIndex = buffer[0];
  buffer[0] = ' ';
  buffer[4] = ' ';
  int input = atoi(buffer);  
  
    if (fIndex == 'R' || fIndex == 'G' || fIndex == 'B')
    {
      if (fIndex == 'R')
        redValue = input;
      if (fIndex == 'G')
        greenValue = input;
      if (fIndex == 'B')
        blueValue = input;

      setColor(redValue, greenValue, blueValue);
    }

    if (fIndex == 'Z')
    {
      strip.setBrightness(input);
    }
strip.show();
}

