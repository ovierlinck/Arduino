#include <arduino.h>

#include "Notifier.h"
#include "Controller.h"

void notify(const char* varName, unsigned char value)
{
  Serial1.print("{\"");
  Serial1.print(varName);  
  Serial1.print("\" : \"");  
  Serial1.print(value);  
  Serial1.print("\"}\n");  
}

void notify(const char* varName, const char* value)
{
  Serial1.print("{\"");
  Serial1.print(varName);  
  Serial1.print("\" : \"");  
  Serial1.print(value);  
  Serial1.print("\"}\n");  
}

void onTemperature(unsigned char temperature)
{
    notify("TEMPERATURE", temperature);
}

void onSetpoint(unsigned char setpoint)
{
    notify("SETPOINT", setpoint);
}

void onCommand(bool command)
{
    notify("COMMAND", command);
}

void onMode(Mode mode)
{
    notify("MODE", getModeName(mode));
}

void onDateTime(uint8_t day, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute, uint8_t second)
{
  notify("DAY", day);
  notify("MONTH", month);
  notify("YEAR", year);
  notify("HOUR", hour);
  notify("MINUTE", minute);
  notify("SECOND", second);
}

