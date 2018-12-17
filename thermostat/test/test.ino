#include "TCurve.h"
#include "TProgram.h"

TCurve curve;
TProgram prg = TProgram("prg");

void testGetSetPoint(byte h, byte m)
{
  byte setPoint = curve.getSetpoint(h, m);
  String msg = "getSetpoint(" ;
  Serial.println(msg + h + ", " + m + ") : " + setPoint);
}
void testProgram()
{
  Serial.println("---------------------------- testProgram()");
  TCurve c1 = TCurve("week");
  c1.setApplicableForDay(0, true);
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

  TCurve* c = prg.getCurveForDay(3);
  Serial.print("Curve for 3 = ");
  Serial.println(c->dump());

  Serial.print("Setpoint for Day 3, 6:35 = ");
  Serial.println(""+c->getSetpoint(6,35));

  TCurve* weekCurves[7];
  prg.getWeekCurves(weekCurves);
  for (char day = 0; day < 7; day++)
  {
    Serial.print("Day ");
    Serial.print(day);
    Serial.print(" : ");
    Serial.println(weekCurves[day]->getName());
  }

}

void setup()
{
  Serial.begin(9600);

  Serial.print("=====================================================");

  curve.setName("Courbe1");
  Serial.println(curve.dump());

  testProgram();
}

int i = 0;

void loop()
{
  int day = (i*16 / 24 / 60) % 7;
  int h = (i*16 / 60) % 24;
  int m = (i*16) % 60;
  String msg = "getSetpoint(";
  Serial.print(  msg+ day + ", " + h + ", " + m + ") : ");
  Serial.println(0 + prg.getSetpoint(day, h, m));
  
  i++;
}

void loop2()
{

  curve.setWeekPattern(i & 127);
  Serial.print(curve.getWeekPattern(), DEC);
  Serial.print(" ");
  Serial.print(curve.getWeekPattern(), BIN);
  Serial.print(" ");
  Serial.println(curve.dump());

  i++;
}

void loop1()
{

  if (i < 7)
  {
    curve.setApplicableForDay(i, true);
  }
  else if (i < 15)
  {
    curve.setApplicableForDay(i, false);
  }

  if (i == 0)
  {
    Serial.println(curve.dump());
    testGetSetPoint(4, 5); // on empty curve
  }
  else if (i < 15) // try more than allowed, to check error handling!
  {
    Serial.println(curve.dump());
    curve.addPoint(i + 20, i + 1, i * 2);
  }
  else if (i < 30)
  {
    if (i % 20 == 0) Serial.println(curve.dump());
    int m = (i - 15);
    byte h = m / 60;
    testGetSetPoint(h, m);
  }

  i++;

}

