/*  DHT11/ DHT22 Sensor Temperature and Humidity Tutorial
    Program made by Dejan Nedelkovski,
    www.HowToMechatronics.com
*/
/*
   You can find the DHT Library from Arduino official website
   http://playground.arduino.cc/Main/DHTLib
*/

#include "dht.h"

#define dataPin1 8 // Defines pin number to which the sensor is connected
#define dataPin2 9 // Defines pin number to which the sensor is connected
dht DHT; // Creats a DHT object

void setup() {

  Serial.begin(9600);
  Serial.print("Start...");
}

void getTH(byte dataPin) {
  int readData = DHT.read11(dataPin); // Reads the data from the sensor
  Serial.print(readData);
  if (readData == DHTLIB_OK)
  {
    float t = DHT.temperature; // Gets the values of the temperature
    float h = DHT.humidity; // Gets the values of the humidity

    // Printing the results on the serial monitor
    //Serial.print("Temperature = ");
    Serial.print(t);
    Serial.print(";");
    //Serial.print("    Humidity = ");
    Serial.print(h);
    Serial.print(";");
  }
  //Serial.println(" % ");
}
void loop() {
  getTH(dataPin1);
  getTH(dataPin2);
  Serial.println();
  
  delay(1000); 
}
