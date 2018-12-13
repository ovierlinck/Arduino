#include "TCurve.h"

#include "Arduino.h"

static char buffer[512];

CurvePoint_t NONE_POINT = TCurve::newCurvePoint(-1, -1, -1);
static const char *const WEEK_STR = "LMMJVSD";

static CurvePoint_t TCurve::newCurvePoint(char temp, char h, char m)
{
  CurvePoint_t point;
  point.temp = temp;
  point.h = h;
  point.m = m;

  return point;
}

TCurve::TCurve()
{
}
TCurve::TCurve(const char* name)
{
  setName(name);
}
void TCurve::setName(const char* name)
{
  strncpy(_name, name, TCurve_LENGTH_NAME);
}

const char* TCurve::getName()
{
  return _name;
}
char TCurve::getNbPoints()
{
  return _nbPoints;
}
void TCurve::addPoint(char temp, char h, char m)
{
  //sprintf(buffer, "TCurve::addPoint(%d, %d, %d)", temp, h, m);
  //Serial.println(buffer);

  addPoint(newCurvePoint(temp, h, m));
}

void TCurve::addPoint(CurvePoint_t p)
{
  //sprintf(buffer, "TCurve::addPoint(%s)", dumpCurvePoint(p));
  //Serial.println(buffer);

  if (_nbPoints >= TCurve_MAX_POINTS)
  {
    Serial.println("ERROR: max NB POINTS reached");
    return;
  }

  _points[_nbPoints++] = p;
}

void TCurve::removeAllPoints()
{
  Serial.println("TCurve::removeAllPoints()");
  _nbPoints = 0;
}

CurvePoint_t& TCurve::getPoint(char index)
{
  if (index < 0 || index >= _nbPoints)
  {
    return NONE_POINT;
  }

  return _points[index];
}

char TCurve::getSetpoint(char h, char m)
{
  char answer = TCurve_NO_SETPOINT;
  char index = 0;
  while (index < _nbPoints && !before(h, m, getPoint(index)))
  {
    answer = getPoint(index).temp;
    index++;
  }
  return answer;
}

void TCurve::setWeekPattern(char pattern)
{
  _weekPattern = pattern;
}

void TCurve::setApplicableForDay(char day, bool applicable)
{
  if (applicable)
  {
    _weekPattern |= (1 << day);
  }
  else
  {
    _weekPattern &= !(1 << day);
  }
}

char TCurve::getWeekPattern()
{
  return _weekPattern;
}

bool TCurve::isApplicableForDay(char day)
{
  return _weekPattern & (1 << day);
}

bool TCurve::before(CurvePoint_t& p1, CurvePoint_t& p2)
{
  return minutes(p1) < minutes(p2);
}
bool TCurve::before(char h, char m, CurvePoint_t& p)
{
  return minutes(h, m) < minutes(p);
}

int TCurve::minutes(CurvePoint_t& p)
{
  return minutes(p.h, p.m);
}

int TCurve::minutes(char h, char m)
{
  return h * 60l + m;
}

const char* TCurve::dumpCurvePoint(CurvePoint_t& p)
{
  static char dump[20];
  sprintf(dump, "{%.2d:%.2d -> %d}", p.h, p.m, p.temp);
  return dump;
}

const char* TCurve::dump()
{
  char temp[5];
  strcpy(buffer, _name);
  strcat(buffer, " {");
  for (int i = 0; i < _nbPoints; i++)
  {
    sprintf(temp, " %d:", i);
    strcat(buffer, temp);
    strcat(buffer, dumpCurvePoint(getPoint(i)));
  }
  strcat(buffer, "} [");
  for (char day = 0; day < 7; day++)
  {
    if (isApplicableForDay(day))
    {
      strcat(buffer, "X");
    }
    else {
      strcat(buffer, ".");
    }
  }
  strcat(buffer, "]");

  return buffer;
}

