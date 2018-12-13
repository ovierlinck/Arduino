void setup() {
  Serial.begin(9600);
}

double i=1;
double j=0;
int compteur = 0;

void loop() {
   Serial.print(compteur);
   Serial.print("  ");
   Serial.print(i);
   Serial.print("  ");
   Serial.print(i/j, 12);
   Serial.println();
   
   double tempo = i;
   i = i+j;
   j = tempo;
 
   compteur ++;

   delay(500);
}
