#define PIN 1
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, PIN);

int g = 0;
void setup() {
  ring.begin();
  ring.show();
  ring.setBrightness(255);
}
 void loop() {
   for (int i = 0; i < 16; i ++){
    ring.setPixelColor(i, 255, 130, 0);
   ring.show();
   g++;
   }
 }
 
 
 


