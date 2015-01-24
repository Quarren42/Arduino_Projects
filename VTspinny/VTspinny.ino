#include <Adafruit_NeoPixel.h>

#define PIN 1


Adafruit_NeoPixel ring = Adafruit_NeoPixel(17, PIN, NEO_GRB + NEO_KHZ800);

uint32_t maroon = ring.Color(255, 0, 0);
uint32_t orange = ring.Color(255, 40, 0);
uint32_t off = ring.Color(0, 0, 0);

int maroonOff = 7;
int orangeOff = 7;

void setup(){
  ring.setBrightness(255);
  ring.begin();
  ring.show();
}
void loop(){
  
  //spin2Color();
  //spin3Color();
  spin4Color();
  

}
void spin2Color(){
   for (int i = 0; i < 16; i ++){
    ring.setPixelColor(i, maroon);
    ring.setPixelColor(((i + 8) % 16), orange);
    ring.setPixelColor(((i + maroonOff) % 16), off);
    ring.setPixelColor(((i + orangeOff + 8) % 16), off);
    ring.show();
    delay (25);
  }
}

void spin3Color(){
   for (int i = 0; i < 16; i ++){
    ring.setPixelColor(i, maroon);
    ring.setPixelColor(((i + 5) % 16), orange);
    ring.setPixelColor(((i + 10) % 16), maroon);
    ring.setPixelColor(((i + maroonOff) % 16), off);
    ring.setPixelColor(((i + orangeOff + 5) % 16), off);
    ring.setPixelColor(((i + maroonOff + 10) % 16), off);
    ring.show();
    delay (100);
  }
}

void spin4Color(){
  for (int i = 0; i < 16; i ++){
    ring.setPixelColor(i, maroon);
    ring.setPixelColor(((i + 4) % 16), orange);
    ring.setPixelColor(((i + 8) % 16), maroon);
    ring.setPixelColor(((i + 12) % 16), orange);
    ring.setPixelColor(((i + maroonOff) % 16), off);
    ring.setPixelColor(((i + orangeOff + 4) % 16), off);
    ring.setPixelColor(((i + maroonOff + 8) % 16), off);
    ring.setPixelColor(((i + orangeOff + 12) % 16), off);
    ring.show();
    delay (50);
  }
}



