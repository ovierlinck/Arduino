int pinA = 6;
int pinB = 7;
int pinC = 8;
int pinD = 9;

int valeur=0;
int duree=10;

void setup() {
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  
  randomSeed(analogRead(0));
  valeur = random(1,7);
  duree = random(100);
}

void de(int v)
{
  digitalWrite(pinA, (v==1 || v==3 || v==5) ? HIGH : LOW);   
  digitalWrite(pinB, (v>1) ? HIGH : LOW);   
  digitalWrite(pinC, (v>3) ? HIGH : LOW);   
  digitalWrite(pinD, (v==6) ? HIGH : LOW);   
}


void loop() {
  
  de(valeur);
  delay(duree);

  if (duree<1000)
  {
     valeur = (valeur+1);
     if (valeur==7) valeur=1;
     duree = duree *1.2;
  }
  
}
