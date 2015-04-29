#include <FastLED.h>

#define NUM_LEDS 16
#define DATA_PIN 1

CRGB leds[NUM_LEDS];

void setup()
{
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop()
{
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = 0x15DA3D;
    FastLED.show();
    leds[i] = CRGB::HotPink;
    delay(30);
  }
}


