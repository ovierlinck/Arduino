int pinA = 6;  // Led centrale
int pinB = 7;  // Coins haut-gauche / bas-droit
int pinC = 8;  // Coins haut-droit / bas-gauche
int pinD = 9;  // Milieux gauche et droite
int pinE = 10; // complement pour finir le 'cercle': milieu haut et bas

int valeur = 1;
int duree = 10;

void setup() {
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);

  randomSeed(analogRead(0));
  valeur = 1;
  duree = random(3, 100);
}

void de(int v)
{
  digitalWrite(pinA, (v == 1 || v == 3 || v == 5 || v == 7 || v == 9) ? HIGH : LOW);
  digitalWrite(pinB, (v > 1) ? HIGH : LOW);
  digitalWrite(pinC, (v > 3) ? HIGH : LOW);
  digitalWrite(pinD, (v > 5) ? HIGH : LOW);
  digitalWrite(pinE, (v > 7) ? HIGH : LOW);

}
// Lancer de dé, ralentit progressivement. Aleatoire.
void lancer() {

  de(valeur);
  delay(duree);

  if (duree < 1000 || valeur != 5)
  {
    valeur = (valeur + 1);
    if (valeur == 10) valeur = 1;
    duree = duree * 1.2;
  }
}

void passageMoins()
{
  valeur -= 1;
  de(valeur);
  delay(500);
}

void passagePlus()
{
  valeur += 1;
  de(valeur);
  delay(500);
}

// De 1 à 6 puis à 1 et on repete
void zigzag()
{
  if (valeur == 6)
  {
    passageMoins();
    passageMoins();
    passageMoins();
    passageMoins();
    passageMoins();
  }
  if (valeur == 1)
  {
    passagePlus();
    passagePlus();
    passagePlus();
    passagePlus();
    passagePlus();
  }
}
int val = 1;
int pas = 1;
void zigzag2()
{
  de(val);
  delay(1000);
  val += pas;
  if (val > 9)
  {
    pas = -pas;
    val += pas;
    val += pas;
  }
  if (val < 1)
  {
    pas = -pas;
    val += pas;;
    val += pas;;
  }
}

// Helice tournante en LEDs
int delaiAvion = 800;
void accelereAvion()
{
  if (delaiAvion > 10) delaiAvion *= 0.95;
}
void avion()
{

  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, HIGH);
  digitalWrite(pinC, LOW);
  digitalWrite(pinD, LOW);
  digitalWrite(pinE, LOW);
  delay (delaiAvion);
  accelereAvion();

  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
  digitalWrite(pinD, HIGH);
  digitalWrite(pinE, LOW);
  delay (delaiAvion);
  accelereAvion();

  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, HIGH);
  digitalWrite(pinD, LOW);
  digitalWrite(pinE, LOW);
  delay (delaiAvion);
  accelereAvion();

  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
  digitalWrite(pinD, LOW);
  digitalWrite(pinE, HIGH);
  delay (delaiAvion);
  accelereAvion();
}

void loop()
{
  //  lancer();
  //zigzag();
  //zigzag2();
  avion();
}
