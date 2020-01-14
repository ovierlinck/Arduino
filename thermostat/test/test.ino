#include <DHT.h>

#include <TCurve.h>
#include <TProgram.h>
#include "Controller.h"
#include "Commands.h"
#include "Notifier.h"

#define RELAY_PIN 11

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#include "DS1307.h"

TProgram prg = TProgram("prg");

Mode mode = AUTO;
unsigned char setpoint = 0;

void createProgram()
{
  Serial.println("Creating Program...");
  TCurve c1 = TCurve("week");
  c1.setApplicableForDay(0, false);
  c1.setApplicableForDay(1, true);
  c1.setApplicableForDay(2, true);
  c1.setApplicableForDay(3, true);
  c1.setApplicableForDay(4, true);
  c1.addPoint(20, 6, 0);
  c1.addPoint(TCurve_SETPOINT_OFF, 8, 0);
  c1.addPoint(21, 19, 0);
  c1.addPoint(15, 21, 0);

  TCurve c2 = TCurve("week-end");
  c2.setApplicableForDay(5, true);
  c2.setApplicableForDay(6, true);
  c2.addPoint(22, 7, 0);
  c2.addPoint(18, 9, 0);
  c2.addPoint(23, 20, 0);
  c2.addPoint(16, 22, 0);

  prg.addCurve(c1);
  prg.addCurve(c2);

  dumpPrg();
}

void dumpPrg()
{
  Serial.println("-- DUMP PRG ----------------------------------");
  for (unsigned char i = 0; i < prg.getNbCurves(); i++)
  {
    Serial.print(i);
    Serial.print(": ");

    TCurve* c = prg.getCurve(i);
    Serial.println(c->dump());
  }
  Serial.println("----------------------------------------------");

}
unsigned char getNbCurves()
{
  return prg.getNbCurves();
}

void deleteAllCurves()
{
  Serial.println("deleteAllCurves()");
  prg.removeAllCurves();
}

void deleteCurve(unsigned char index)
{
  Serial.println(String("deleteCurve(") + index + ")");
  prg.removeCurve(index);
}

unsigned char addCurve(TCurve c)
{
  Serial.println(String("addCurve(") + c.getName() + ")");
  prg.addCurve(c);

  return prg.getNbCurves();
}

TCurve* getCurve(unsigned char index)
{
  return prg.getCurve(index);
}

// Read sensor temperature DHT22 on pin DHT22_PIN and returns T (or -1 if error)
float getTemperature()
{
  float t = dht.readTemperature();
  if (isnan(t))
  {
    return -1.0;
  }
  return t;
}

void setMode(Mode pMode)
{
  mode = pMode;
  Serial.println(String("MODE ") + getModeName(mode));
  if (mode == OFF)
  {
    setSetpoint(0);
  }
  onMode(mode);
}

Mode getMode()
{
  return mode;
}

void setSetpoint(unsigned char pSetpoint)
{
  if (setpoint != pSetpoint)
  {
    Serial.println(String("Setpoint ") + pSetpoint);
  }
  setpoint = pSetpoint;
  onSetpoint(setpoint);
}

unsigned char getSetpoint()
{
  return setpoint;
}

void setDate(unsigned char days, unsigned char months, unsigned int year, unsigned char dayOfWeek)
{
  Serial.println(String("setDate(") + days + "/" + months + "/" + year + ":" + dayOfWeek + ")");
  set_current_date(days, months, year, dayOfWeek);
}

void setTime(unsigned char hours, unsigned char minutes)
{
  Serial.println(String("setTime(") + hours + ":" + minutes + ")");
  set_current_time(hours, minutes);
}

bool getDate(unsigned char& days, unsigned char& months, unsigned int& year, unsigned char& dayOfWeek)
{
  DateTime_t now;
  if (read_current_datetime(&now))
  {
    return false;
  }
  days = now.days;
  months = now.months;
  year = now.year;
  dayOfWeek = now.day_of_week;

  return true;
}

bool getTime(unsigned char& hours, unsigned char& minutes)
{
  DateTime_t now;
  if (read_current_datetime(&now))
  {
    return false;
  }
  hours = now.hours;
  minutes = now.minutes;

  return true;
}

const char* getModeName(Mode mode)
{
  switch (mode)
  {
    case OFF: return "OFF";
    case AUTO: return "AUTO";
    case MANUAL: return "MANUAL";
    default: return "**UNKNOWN**";
  }
}

/*
  Switch ON:
  - mode off
  - check clock. If OK : mode auto

*/


void setup()
{
  Serial.begin(9600);
  Serial1.begin(38400);

  Serial.println("== SETUP ========================================================");

  pinMode(RELAY_PIN, OUTPUT);
  dht.begin();

  createProgram();

  installCommands();

  delay(1000); // To let time for clock to initialize!
  initializeRTClock();

  Serial.println("== SETUP DONE ===================================================");

}

void loop()
{
  processCommands();

  DateTime_t now;
  bool clockOK = true;
  if (read_current_datetime(&now))
  {
    Serial.print("NO CLOCK!");
    clockOK = false;
  }
  else
  {
    onDateTime(now.days, now.months, now.year, now.hours, now.minutes, now.seconds);
  }

  if (mode == AUTO)
  {
    if (!clockOK)
    {
      Serial.print("AUTO: NO CLOCK!");
      setSetpoint(0);
    }
    else
    {
      setSetpoint(prg.getSetpoint(now.day_of_week, now.hours, now.minutes));

      Serial.print(  String("AUTO: getSetpoint(") + (uint8_t)now.days + "/" + (uint8_t)now.months + "/" + (uint8_t)now.year + " "
                     + (uint8_t)now.hours + "H" + (uint8_t)now.minutes + ", Day" + (uint8_t)now.day_of_week + ")" );
    }
  }
  else
  {
    Serial.print(mode == OFF ? "OFF" : "MANUAL");
    delay(50); // Otherwise, too fast for T reading
  }

  onMode(mode);
  
  onSetpoint(setpoint);

  float temp = getTemperature();
  onTemperature(temp);

  bool command = (temp >= 0 ? (temp < setpoint) : false);
  onCommand(command);

  Serial.println(String("\tsetPoint:") + (uint8_t)setpoint + "°\tTemp=" + temp + "°\tCommand=" + command + (command ? "=LOW" : "=HIGH"));

  digitalWrite(RELAY_PIN , command ? LOW : HIGH);

  delay(1000);
}


