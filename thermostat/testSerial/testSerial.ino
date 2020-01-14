int inByte = 0;         // incoming serial byte

void setup() 
{
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    delay(100);
  }

  Serial1.begin(9600);
  while (Serial1.available() <= 0)
  {
    Serial.println("Nothing on Serial 1");   // send an initial string
    delay(300);
  }
}

void loop() 
{
  
  // if we get a valid byte, read analog ins:
  if (Serial1.available() > 0) {
    inByte = Serial1.read();

    Serial.print("Received ");
    Serial.println(inByte);
  }
}

