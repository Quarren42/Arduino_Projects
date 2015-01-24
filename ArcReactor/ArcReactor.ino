
#include <Adafruit_NeoPixel.h>
 
#define PIN 1
 

Adafruit_NeoPixel strip = Adafruit_NeoPixel(17, PIN, NEO_GRB + NEO_KHZ800);
 
int alpha; // Current value of the pixels

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
void loop(){
  pulsecolour();
}

//pulse a specific colour - i = the min and max brightness / range
void pulsecolour(){                             
for (int i = 35; i <= 255; i++){ 
 colorWipe(strip.Color(0, 0, i));
}
for (int i = 255; i >= 35; i--){
   colorWipe(strip.Color(0, 0, i));
}
}

 // Fill the dots one after the other with a color
void colorWipe(uint32_t c) { 
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}
