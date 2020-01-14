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
  LEDS.setBrightness(50);
}

void loopGoutte() {
  int pause = 100;
  for (int pos = 0; pos < NUM_LEDS / 2; pos++)
  {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    CHSV color  = CHSV(pos * 2 * 255 / NUM_LEDS, 255, 255 - (pos * 255 / NUM_LEDS));
    leds[NUM_LEDS / 2 + pos] = color;
    leds[NUM_LEDS / 2 - 1 - pos] = color;

    FastLED.show();
    delay(pause / 50);
    pause = pause * 1.2;
  }
}

int pause = 40;
int color = 0;
int pos = 0;
int vitesse = 1;

void mettreLed(int pos, int h, int s, int v)
{
  if (pos >= 0 && pos < NUM_LEDS)
  {
    leds[pos] = CHSV(h, s, v);
  }
}
void loopChenille()
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  for (int oeuf = 0; oeuf < pos ; oeuf += 10)
  {
    //mettreLed(oeuf, 0, 0, ( pos - oeuf) * 4 );
    mettreLed(oeuf, 0, 0, ((vitesse > 0 ? pos : NUM_LEDS) - oeuf) * 4 );
  }

  mettreLed(pos, 85, 255, 255);
  mettreLed(pos - vitesse, (color + 85) / 2, 255, 125);
  mettreLed(pos - 2 * vitesse, color, 255, 125);
  mettreLed(pos - 3 * vitesse, (color - 85) / 2, 255, 125);


  pos = (pos + vitesse);
  if (pos > NUM_LEDS || pos < 0)
  {
    vitesse = -vitesse;
  }
  color += 2;
  FastLED.show();
  delay(pause);
}

int step = 0;
void loopGuirlande()
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (int pos = 0; pos < NUM_LEDS; pos+=4)
  {
    if (step < 3)
    {
      leds[pos] = CHSV(0, 255, 200);
      leds[pos + 2] = CHSV(60, 255, 200);
    }
    else
    {
      leds[pos + 1] = CHSV(120, 255, 200);
      leds[pos + 3] = CHSV(180, 255, 200);
    }
  }

  step = (step + 1) % 6;

  FastLED.show();
  delay(30);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(120);
  if (step%3==0) delay(80);
}

void loop()
{
  //  loopGoutte();
  //  loopChenille();
  loopGuirlande();
}

