#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, 7);
Adafruit_NeoPixel left = Adafruit_NeoPixel(6, 2);
Adafruit_NeoPixel right = Adafruit_NeoPixel(6, 3);
Adafruit_NeoPixel tail = Adafruit_NeoPixel(7, 4);

int incomingByte = 0;
int incomingByte2 = 0;
int incomingByte3 = 0;
int brightness = 0;

uint32_t red = ring.Color(255, 0, 0);
uint32_t green = ring.Color(0, 255, 0);
uint32_t blue = ring.Color(0, 0, 255);
uint32_t yellow = ring.Color(255, 255, 0);
uint32_t cyan = ring.Color(0, 255, 255);
uint32_t purple = ring.Color(255, 0, 255);
uint32_t white = ring.Color(255, 255, 255);

uint32_t off = ring.Color(0, 0, 0);

void setup() {

  pinMode(7, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(9600);
  ring.show();
  
  for (int i = 0; i < 7; i++){
    tail.setPixelColor(i, 255, 0, 0);
    left.setPixelColor(i, 255, 255, 255);
    right.setPixelColor(i, 255, 255, 255);
}
tail.setBrightness(0);
left.setBrightness(0);
right.setBrightness(0);
tail.show();
left.show();
right.show();

}
void loop() {
  char buffer[] = {' ', ' ', ' ', ' ', ' ', ' ', ' '};
  while (!Serial.available());
  Serial.readBytesUntil('\n', buffer, 7);
  int incomingByte = atoi(buffer);
  Serial.println(incomingByte);

  switch (incomingByte){

  case 1:
    {
      char buffer2[] = {' ',' ',' ',' ',' ',' ',' '}; 
      while (!Serial.available());
      Serial.readBytesUntil('\n', buffer2, 7);
      incomingByte2 = atoi(buffer2);
    }

    switch (incomingByte2){

    case 0:
      for (int i = 0; i < 16; i++){
        ring.setPixelColor(i, red);
        ring.show();
      }
      break;

    case 1:
      for (int i = 0; i < 16; i++){
        ring.setPixelColor(i, green);
        ring.show();
      }
      break;

    case 2:
      for (int i = 0; i < 16; i++){
        ring.setPixelColor(i, blue);
        ring.show();
      }
      break;

    case 3:
      for (int i = 0; i < 16; i++){
        ring.setPixelColor(i, yellow);
        ring.show();
      }
      break;

    case 4:
      for (int i = 0; i < 16; i++){
        ring.setPixelColor(i, cyan);
        ring.show();
      }
      break;

    case 5:
      for (int i = 0; i < 16; i++){
        ring.setPixelColor(i, purple);
        ring.show();
      }
      break;

    case 6:
      for (int i = 0; i < 16; i++){
        ring.setPixelColor(i, white);
        ring.show();
      }
      break;

    case 7:
      for (int i = 0; i < 16; i++){
        ring.setPixelColor(i, off);
        ring.show();
      }
      break;


    }
    break;


  case 2:
    {
      char buffer3[] = {' ', ' ', ' ', ' '};
      while (!Serial.available());
      Serial.readBytesUntil('\n', buffer3, 4);
      incomingByte3 = atoi(buffer3);
      ring.setBrightness(incomingByte3);
      ring.show();
    }
    break;
  }
}















