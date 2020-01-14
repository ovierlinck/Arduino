#include "Arduino.h"
#include "DS1307.h"
#include <Wire.h>

/** Fonction de conversion BCD -> decimal */
byte bcd_to_decimal(byte bcd) {
  return (bcd / 16 * 10) + (bcd % 16);
}
/** Fonction de conversion decimal -> BCD */
byte decimal_to_bcd(byte decimal) {
  return (decimal / 10 * 16) + (decimal % 10);
}

/**
   Fonction récupérant l'heure et la date courante à partir du module RTC.
   Place les valeurs lues dans la structure passée en argument (par pointeur).
   N.B. Retourne
    1 si le module RTC est arrêté (plus de batterie, horloge arrêtée manuellement, etc.),
    -1 si la connection a echoue
    0 le reste du temps.
*/
byte read_current_datetime(DateTime_t *datetime)
{

  /* Début de la transaction I2C */
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write((byte) 0); // Lecture mémoire à l'adresse 0x00
  byte endTr = Wire.endTransmission(); // Fin de la transaction I2C

  if (endTr)
  {
    Serial.println(String("Unable to talk to clock: endTransmission returned ") + endTr);
    return -1;
  }
  
  /* Lit 7 octets depuis la mémoire du module RTC */
  Wire.requestFrom(DS1307_ADDRESS, (byte) 7);
  byte raw_seconds = Wire.read();
  datetime->seconds = bcd_to_decimal(raw_seconds);
  datetime->minutes = bcd_to_decimal(Wire.read());
  byte raw_hours = Wire.read();
  if (raw_hours & 64) { // Format 12h
    datetime->hours = bcd_to_decimal(raw_hours & 31);
    datetime->is_pm = raw_hours & 32;
  } else { // Format 24h
    datetime->hours = bcd_to_decimal(raw_hours & 63);
    datetime->is_pm = 0;
  }
  datetime->day_of_week = bcd_to_decimal(Wire.read());
  datetime->days = bcd_to_decimal(Wire.read());
  datetime->months = bcd_to_decimal(Wire.read());
  datetime->year = bcd_to_decimal(Wire.read());

  /* Si le bit 7 des secondes == 1 : le module RTC est arrêté */
  return raw_seconds & 128;
}

/**
   Fonction ajustant l'heure et la date courante du module RTC à partir des informations fournies.
   N.B. Redémarre l'horloge du module RTC si nécessaire.
*/
void set_current_datetime(DateTime_t *datetime)
{

  /* Début de la transaction I2C */
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write((byte) 0); // Ecriture mémoire à l'adresse 0x00
  Wire.write(decimal_to_bcd(datetime->seconds) & 127); // CH = 0
  Wire.write(decimal_to_bcd(datetime->minutes));
  Wire.write(decimal_to_bcd(datetime->hours) & 63); // Mode 24h
  Wire.write(decimal_to_bcd(datetime->day_of_week));
  Wire.write(decimal_to_bcd(datetime->days));
  Wire.write(decimal_to_bcd(datetime->months));
  Wire.write(decimal_to_bcd(datetime->year));
  Wire.endTransmission(); // Fin de transaction I2C
}

void set_current_date(unsigned char days, unsigned char months, unsigned int year, unsigned char day_of_week)
{
  DateTime_t now;
  if (read_current_datetime(&now))
  {
    Serial.println(F("L'horloge du module RTC n'est pas active !"));
    return;
  }

  now.days  = days;
  now.months = months;
  now.year = year;
  now.day_of_week = day_of_week;

  /* Début de la transaction I2C */
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write((byte) 0); // Ecriture mémoire à l'adresse 0x00
  Wire.write(decimal_to_bcd(now.seconds) & 127); // CH = 0
  Wire.write(decimal_to_bcd(now.minutes));
  Wire.write(decimal_to_bcd(now.hours) & 63); // Mode 24h
  Wire.write(decimal_to_bcd(now.day_of_week));
  Wire.write(decimal_to_bcd(now.days));
  Wire.write(decimal_to_bcd(now.months));
  Wire.write(decimal_to_bcd(now.year));
  Wire.endTransmission(); // Fin de transaction I2C
}

void set_current_time(unsigned char hours, unsigned char minutes)
{
  DateTime_t now;
  if (read_current_datetime(&now))
  {
    Serial.println(F("L'horloge du module RTC n'est pas active !"));
    return;
  }

  now.hours  = hours;
  now.minutes = minutes;
  now.seconds = 0;

  /* Début de la transaction I2C */
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write((byte) 0); // Ecriture mémoire à l'adresse 0x00
  Wire.write(decimal_to_bcd(now.seconds) & 127); // CH = 0
  Wire.write(decimal_to_bcd(now.minutes));
  Wire.write(decimal_to_bcd(now.hours) & 63); // Mode 24h
  Wire.write(decimal_to_bcd(now.day_of_week));
  Wire.write(decimal_to_bcd(now.days));
  Wire.write(decimal_to_bcd(now.months));
  Wire.write(decimal_to_bcd(now.year));
  Wire.endTransmission(); // Fin de transaction I2C
}



byte initializeRTClock()
{
  Wire.begin();

  /* Vérifie si le module RTC est initialisé */
  DateTime_t now;

  char res = read_current_datetime(&now);

  if (res != 0)
  {
    Serial.println(F("L'horloge du module RTC n'est pas active ! Configuring"));

    // Reconfiguration avec une date et heure en dure (pour l'exemple)
    
      now.seconds = 0;
      now.minutes = 39;
      now.hours = 2; // 12h 0min 0sec
      now.is_pm = 1;
      now.day_of_week = 5;
      now.days = 1;
      now.months = 2;
      now.year = 19;
      set_current_datetime(&now);
    
    return 1;
  }

  Serial.println(String("Clock is active and already set : ") + now.days + "/" + now.months + "/" + now.year + " " + now.hours + ":" + now.minutes + "'" + now.seconds + "\" day of week=" + now.day_of_week);
}
