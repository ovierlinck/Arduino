int pin = 7;

void point() {
  digitalWrite(pin, HIGH);   
  delay(100);             
  digitalWrite(pin, LOW);    
  delay(200);             
}

void trait() {
  digitalWrite(pin, HIGH);   
  delay(600);             
  digitalWrite(pin, LOW);    
  delay(200);             
}

void lettre(String code) {
  for (int i=0; i<code.length(); i++)
  {
    char signe = code.charAt(i);
    if (signe=='.')
    {
      point(); 
    }
    else
    {
      trait();
    }
  }
  delay(1000);

}

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(pin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  
  lettre(".-.");  // R
  lettre("---");  // O
  lettre("-..."); // B
  lettre("..");   // I
  lettre("-.");   // N

  delay(1000);
  
  lettre(".");
  lettre("..");
  lettre("...");
  lettre("....");
  lettre(".....");
  lettre("......");
  lettre(".......");
  lettre("........");

  delay(1000);

}
