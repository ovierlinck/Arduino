// From https://www.brainy-bits.com/connect-a-character-lcd-using-the-i2c-bus/
#include "Wire.h"
#include "LCD.h"
#include "LiquidCrystal_I2C.h"
#include "NewPing.h"

#define I2C_ADDR 0x27  // Add your address here.
#define Rs_pin 0
#define Rw_pin 1
#define En_pin 2
#define BACKLIGHT_PIN 3
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

#define ECHO_PIN 11 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN 12 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

/*
 * ATTENTION: les labels des pins sont FAUX sur la carte: R= green / G=red !!! (B= blue)
 */
#define redpin  2 // select the pin for the red LED
#define greenpin 3 // select the pin for the green LED
#define bluepin  4 // select the pin for the  blue LED

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

void setup()
{
  Serial.begin(9600);

  lcd.begin (20, 4); //  our LCD is a 20x4, change for your LCD if needed

  // LCD Backlight ON
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.home (); // go home on LCD
  lcd.print("Range Finder HC-SR04");

  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
}
#define MAX_LED 64 // Valeur la plus lumineuse pour une composante RGB de la LED
#define MAX_VAL MAX_DISTANCE // Valeur la plus grande de l'echelle de couleur


int rouge(float val)
{
  if (val < MAX_VAL / 3) return MAX_LED * (1 - val * 3 / MAX_VAL);
  if (val > 2 * MAX_VAL / 3) return MAX_LED * (val * 3 / MAX_VAL - 2);
  return 0;
}
int vert(float val)
{
  if (val > 2 * MAX_VAL / 3) return MAX_LED * (3 - val * 3 / MAX_VAL);
  if (val < MAX_VAL / 3) return 0;
  return MAX_LED * (val * 3 / MAX_VAL - 1);
}
int bleue(float val)
{
  if (val > 2 * MAX_VAL / 3) return 0;
  if (val < MAX_VAL / 3) return MAX_LED * (val * 3 / MAX_VAL);
  return MAX_LED * (2 - val * 3 / MAX_VAL);
}
void loop()
{
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  unsigned int cm = sonar.convert_cm(uS); // Convert into centimeters

  Serial.print("us = ");
  Serial.print(uS);
  Serial.print("µs - cm = ");
  Serial.print(cm);
//  Serial.print("  -  ");
//  Serial.print(uS * 1.0 / US_ROUNDTRIP_CM);
  Serial.print("cm (");
  Serial.print(rouge(cm));
  Serial.print(",");
  Serial.print(vert(cm));
  Serial.print(",");
  Serial.print(bleue(cm));
  Serial.println(")");


  lcd.setCursor (0, 1); // go to start of 2nd line
  lcd.print("Current Distance:");
  lcd.setCursor (0, 3); // go to start of 4th line
  lcd.print("Ping: ");
  lcd.print(cm);
  lcd.print(" cm ");

  analogWrite(redpin, rouge(cm));
  analogWrite(greenpin, vert(cm));
  analogWrite(bluepin, bleue(cm));

  delay(50);
}
