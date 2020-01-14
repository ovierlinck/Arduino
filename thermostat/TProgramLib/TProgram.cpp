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
    return false;
  }

  if (curve.getWeekPattern() == 0)
  {
    Serial.println("ERROR on addCurve(): Curve must be applicable for at least one day");
    return false;
  }
  
  if (curve.getSetpoint(23, 59) == TCurve_NO_SETPOINT)
  {
    Serial.println("ERROR on addCurve(): Curve must not be empty");
    return false;
  }

  for (int day = 0; day < 7; day++)
  {
    if (curve.isApplicableForDay(day) && getCurveForDay(day) != NULL)
    {
      Serial.print("ERROR on addCurve(): Curve already defined for one of the days: ");
      Serial.println(day);
      return false;
    }
  }
  _curves[_nbCurves++] = curve;
  
  return true;
}

void TProgram::removeAllCurves()
{
  _nbCurves = 0;
}

unsigned char TProgram::getNbCurves()
{
  return _nbCurves;
}

void TProgram::removeCurve(unsigned char index)
{
	if (index>=_nbCurves)
	{
		return;
	}
	for (unsigned char i=index;i<_nbCurves-1;i++)
	{
		_curves[i] = _curves[i+1];
	}
	_nbCurves--;
}

TCurve* TProgram::getCurve(unsigned char index)
{
  if (index < 0 || index >= _nbCurves)
  {
    return NULL;
  }

  return &_curves[index];
}

unsigned char TProgram::getSetpoint(unsigned char day, unsigned char h, unsigned char m)
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
      unsigned char setpoint = curve->getSetpoint(h, m);
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
/*
void TProgram::getWeekCurves(TCurve* curves[7] )
{
  for (unsigned char day = 0; day < 7; day++)
  {
    curves[day] = getCurveForDay(day);
  }
}
*/

TCurve* TProgram::getCurveForDay(unsigned char day)
{
  for (unsigned char i = 0; i < _nbCurves; i++)
  {
    if (_curves[i].isApplicableForDay(day))
    {
      return &_curves[i];
    }
  }

  return NULL;
}


