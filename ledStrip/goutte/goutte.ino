#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 60

#define DATA_PIN 5// Green=data / red=5V / white=ground

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(84);
}

void loop() {
  int pause = 100;
  for (int pos = 0; pos < NUM_LEDS / 2; pos++)
  {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    CHSV color  = CHSV(pos * 2 * 255 / NUM_LEDS, 255, 255 - (pos * 255 / NUM_LEDS));
    leds[NUM_LEDS / 2 + pos] = color;
    leds[NUM_LEDS / 2 - pos] = color;

    FastLED.show();
    delay(pause / 50);
    pause *= 1.2;
  }
}


