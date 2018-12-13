#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 60

#define DATA_PIN 5  // Green=data / red=5V / white=ground

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(84);
}

void loop() {
  static uint8_t hue = 0;
  static int pos = 0;

  fill_solid(leds, NUM_LEDS, CHSV(255-hue, 200, 200));//CRGB::Black);
  leds[pos] = CHSV(hue, 255, 255);

  FastLED.show();
  delay(20);
  pos = (pos + 1) % NUM_LEDS;
  hue = (hue + 1) % 256;

}


