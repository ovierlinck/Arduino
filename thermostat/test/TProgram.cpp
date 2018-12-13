#include "TProgram.h"

#include "Arduino.h"

TProgram::TProgram()
{}

TProgram::TProgram(const char* name)
{}

void TProgram::setName(const char* name)
{
  strncpy(_name, name, TPROGRAM_LENGTH_NAME);
}

const char* TProgram::getName()
{
  return _name;
}

bool TProgram::addCurve(TCurve& curve)
{
  if (_nbCurves > TPROGRAM_MAX_CURVES)
  {
    Serial.println("ERROR: max nb curves reached");
    return;
  }

  if (curve.getWeekPattern() == 0)
  {
    Serial.println("ERROR on addCurve(): Curve must be applicable for at least one day");
    return;
  }
  if (curve.getSetpoint(23, 59) == TCurve_NO_SETPOINT)
  {
    Serial.println("ERROR on addCurve(): Curve must not be empty");
    return;
  }

  for (int day = 0; day < 7; day++)
  {
    if (curve.isApplicableForDay(day) && getCurve(day) != NULL)
    {
      Serial.print("ERROR on addCurve(): Curve already defined for one of the days: ");
      Serial.println(day);
      return;
    }
  }
  _curves[_nbCurves++] = curve;
}

void TProgram::removeAllCurves()
{
  _nbCurves = 0;
}

char TProgram::getNbCurves()
{
  return _nbCurves;
}

TCurve* TProgram::getCurve(char index)
{
  if (index < 0 || index >= _nbCurves)
  {
    return NULL;
  }

  return &_curves[index];
}

char TProgram::getSetpoint(char day, char h, char m)
{
  if (_nbCurves == 0)
  {
    Serial.println("ERROR on getSetpoint(): no curves defined for this program");
    return TCurve_NO_SETPOINT;
  }

  while (true) //Must end at sometime: TProgram must have at least one curve, curve can not be empty
  {

    TCurve* curve = getCurveForDay(day);
    if (curve != NULL)
    {
      char setpoint = curve->getSetpoint(h, m);
      if (setpoint != TCurve_NO_SETPOINT)
      {
        return setpoint;
      }
    }
    
    // After attempt at 'normal' time, look at end of previous day(s) for last trailing setpoint.
    day = (day-1)%7;
    h=23;
    m=59;
  }
}
/** returns and array of curves, one per day of the week, 1 curve per day, 
applicable for that day. One curve may be present for multiple days*/
void TProgram::getWeekCurves(TCurve* curves[7] )
{
  for (char day = 0; day < 7; day++)
  {
    curves[day] = getCurveForDay(day);
  }
}

TCurve* TProgram::getCurveForDay(char day)
{
  for (char i = 0; i < _nbCurves; i++)
  {
    if (_curves[i].isApplicableForDay(day))
    {
      return &_curves[i];
    }
  }

  return NULL;
}


