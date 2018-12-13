/* Brancher le capteur de distance HC-SR04, le placer en debut de ledstrip
Les leds s'allument en arc-en-ciel jusqu'a la main placee sur le strip!
*/

#include <FastLED.h>
#include "NewPing.h"

// How many leds in your strip?
#define NUM_LEDS 60
#define DATA_PIN 5 // Green=data / red=5V / white=ground

// Define the array of leds
CRGB leds[NUM_LEDS];


#define ECHO_PIN 11 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN 12 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define MAX_DISTANCE 150 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(84);
}
void loop()
{
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  unsigned int cm = sonar.convert_cm(uS); // Convert into centimeters
  Serial.print("us = ");
  Serial.print(uS);
  Serial.print("µs - cm = ");
  Serial.print(cm);
  Serial.print("  -  ");
  Serial.print(uS*1.0/US_ROUNDTRIP_CM);
  Serial.println("cm");

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  int n = min(1.0*cm/1.7,NUM_LEDS);
  for (int pos = 0; pos < n; pos++)
  {
    hsv2rgb_spectrum(CHSV(pos*255/n, 240, 200), leds[pos]);
  }
  FastLED.show();

  delay(100);
}

void loopShoot()
{
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  unsigned int cm = sonar.convert_cm(uS); // Convert into centimeters
  Serial.print("us = ");
  Serial.print(uS);
  Serial.print("µs - cm = ");
  Serial.print(cm);
  Serial.print("  -  ");
  Serial.print(uS*1.0/US_ROUNDTRIP_CM);
  Serial.println("cm");

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  int n = min(1.0*cm/1.7,NUM_LEDS);
  for (int pos = 0; pos < n; pos++)
  {
    hsv2rgb_spectrum(CHSV(pos*255/n, 240, 200), leds[pos]);
    FastLED.show();
    leds[pos]=CRGB::Black;
    delay(1);
  }
}



