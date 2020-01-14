#include "Commands.h"
#include "Controller.h"
#include <TCurve.h>

#include "Arduino.h"
#include <SerialCommands.h>

char serial_command_buffer_[50];
char serial1_command_buffer_[50];

SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\n", " ");
SerialCommands serial1_commands_(&Serial1, serial1_command_buffer_, sizeof(serial1_command_buffer_), "\n", " ");

//This is the default handler, and gets called when no other command matches.
void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
  sender->GetSerial()->print("Unrecognized command [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("]");
}

/** Dump curve to serial.
  Output: CURVE name pattern (h m t)* /
*/
void dumpCurve(Stream* serial, unsigned char index, TCurve* c)
{
  serial->print(String("{ \"Curve\" : { \"Index\" : \"") + index + "\", \"Name\" : \"" + c->getName() + "\", \"Pattern\": \"" + c->getWeekPattern() + "\", \"Points\":[ ");
  unsigned char nbPt = c->getNbPoints();
  for (int i = 0; i < nbPt; i++)
  {
    if (i>0)
    {
      serial->print(", ");
    }
    CurvePoint_t point = c->getPoint(i);
    serial->print(String("{ \"hour\":\"") + point.h + "\",\"Minute\":\"" + point.m + "\",\"Setpoint\":\" " + point.temp + "\"}");
  }
  serial->println("]}}");
}

/** Dump curve to serial.
  Output: CURVE name pattern (h m t)* /
*/
void dumpCurveByIndex(Stream* serial, unsigned char index)
{
  TCurve* c = getCurve(index);
  if (c == NULL)
  {
    serial->println(String("ERROR: no curve with index ") + index);
    return;
  }
  dumpCurve(serial, index, c);
}

void error(Stream* serial, char* msg)
{
  serial->println(String("{\"Error\" : \"") + msg + "\"");
}

/**
   Goes to AUTO mode (follow prg)
  Input: nothing
  Output: nothing
*/
void cmd_auto()
{
  setMode(AUTO);
}

/**
   Goes to OFF mode
  Input: nothing
  Output: nothing
*/
void cmd_off()
{
  setMode(OFF);
}

/**
   Goes to MANUAL mode
  Input: setpoint
  Output: nothing
*/
void cmd_manu(SerialCommands* sender)
{
  char* setStr = sender->Next();
  if (setStr == NULL)
  {
    error(sender->GetSerial(), "MANU but no setpoint");
    return;
  }

  setMode(MANUAL);
  setSetpoint(atoi(setStr));
}

/* Set current time.
  Input: HH MM
  Output: nothing
*/
void cmd_setTime(SerialCommands* sender)
{
  char* str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "TIME but no HOUR");
    return;
  }

  unsigned char hour = atoi(str);

  str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "TIME but no MINUTE");
    return;
  }

  unsigned char minute = atoi(str);

  setTime(hour, minute);
}

/* Set current date.
  Input: dd mm yyyy dayOfWeek
  Output: nothing
*/
void cmd_setDate(SerialCommands* sender)
{
  char* str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "DATE but no DAY");
    return;
  }

  unsigned char day = atoi(str);

  str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "DATE but no MONTH");
    return;
  }

  unsigned char month = atoi(str);

  str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "DATE but no YEAR");
    return;
  }

  unsigned char year = atoi(str);

  str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "DATE but no DAY_OF_WEEK");
    return;
  }

  unsigned char dayOfWeek = atoi(str);

  setDate(day, month, year, dayOfWeek);
}

/** Returns current date on input serial.
  Input: nothing
  Ouput:  DATE DD/MM/YYYY:dow
*/
void cmd_getDate(SerialCommands* sender)
{
  unsigned char days, months, dayOfWeek;
  unsigned int year;

  if (!getDate(days, months, year, dayOfWeek))
  {
    error(sender->GetSerial(), "Unable to get date");
    return;
  }
  sender->GetSerial()->println(String("DATE ") + days + "/" + months + "/" + year + ":" + dayOfWeek);
}

/** Returns current time on input serial.
  Input: nothing
  Output: TIME HH:mm
*/
void cmd_getTime(SerialCommands* sender)
{
  unsigned char h, m;
  if (!getTime(h, m))
  {
    error(sender->GetSerial(), "Unable to get time");
    return;
  }
  sender->GetSerial()->println(String("TIME ") + h + ":" + m);
}

/** Returns current setpoint on input serial.
  Input: nothing
  Output: SETPOINT T
*/
void cmd_getSetpoint(SerialCommands* sender)
{
  sender->GetSerial()->println(String("SETPOINT ") + getSetpoint());
}

/** Returns current MODE on input serial.
  Input: nothing
  Ouput: MODE M
*/
void cmd_getMode(SerialCommands* sender)
{
  sender->GetSerial()->println(String("MODE ") + getModeName(getMode()));
}

/** Returns current temperature on input serial.
  Input: nothing
  Ouput: TEMPERATURE T
*/
void cmd_getTemperature(SerialCommands* sender)
{
  sender->GetSerial()->println(String("TEMPERATURE ") + getTemperature());
}

/** Returns new number of curves on input serial.
  Input: nothing
  Output: NB_CURVES n
*/
unsigned char cmd_getNbCurves(SerialCommands* sender)
{
  sender->GetSerial()->println(String("NB_CURVES ") + getNbCurves());
}

/** Remove all curves - Returns new number of curves on input serial.
  Input: nothing
  Ouput: NB_CURVES n
*/
void cmd_delCurves(SerialCommands* sender)
{
  deleteAllCurves();
  sender->GetSerial()->println(String("NB_CURVES ") + getNbCurves());
}

/** Delete given curve by index - Returns new number of curves on input serial.
  Input: index
  Output: NB_CURVES n
*/
void cmd_delCurve(SerialCommands* sender)
{
  char* str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "delCurve without curve index");
    return;
  }

  unsigned char index = atoi(str);

  deleteCurve(index);
  sender->GetSerial()->println(String("NB_CURVES ") + getNbCurves());
}
void echo(char * str)
{
  return; // SKIP!
  if (str == NULL)
  {
    Serial.println("Found NULL");
  }
  else
  {
    Serial.println(String("Found #") + str + "#");
  }
}

/** Set all setpoints for curve given by index (remove all previously existing setpoints) */
void parseAndSetCurvePoints(TCurve* c, SerialCommands* sender)
{
  c->removeAllPoints();
  char* str;
  while (true)
  {
    str = sender->Next();
    echo(str);

    if (!strcmp("/", str)) // Finished
    {
      return;
    }

    if (strcmp("(", str))
    {
      error(sender->GetSerial(), "missing opening '('");
      return;
    }

    str = sender->Next();
    echo(str);
    if (str == NULL)
    {
      error(sender->GetSerial(), "missing hour");
      return;
    }

    unsigned char hour = atoi(str);

    str = sender->Next();
    echo(str);
    if (str == NULL)
    {
      error(sender->GetSerial(), "missing minute");
      return;
    }

    unsigned char minute = atoi(str);

    str = sender->Next();
    echo(str);
    if (str == NULL)
    {
      error(sender->GetSerial(), "missing temperature");
      return;
    }

    unsigned char temp = atoi(str);

    str = sender->Next();
    echo(str);
    if (strcmp(")", str))
    {
      error(sender->GetSerial(), "missing closing ')'");
      return;
    }

    sender->GetSerial()->println(String("DEBUG: adding point (") + hour + ":" + minute + " t=" + temp + "°)");
    c->addPoint(temp, hour, minute);
  }
  dumpCurve(sender->GetSerial(), 99, c);
}

/** Add one empty curve with given name. Returns new number of curves on input serial.
  Input: curveName pattern (h m t)* /
  Ouput: NB_CURVES n
*/
void cmd_addCurve(SerialCommands* sender)
{
  char* str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "addCurve without curve name");
    return;
  }

  TCurve curve;
  curve.setName(str);

  str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "addCurve without week pattern");
    return;
  }

  unsigned char pattern = atoi(str);
  curve.setWeekPattern(pattern);

  parseAndSetCurvePoints(&curve, sender);

  addCurve(curve);

  sender->GetSerial()->println(String("NB_CURVES ") + getNbCurves());
}


/** Return given curve by index .
  Input: index
  Output: CURVE name pattern (h m t)* /
*/
void cmd_getCurve(SerialCommands* sender)
{
  char* str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "getCurve without curve index");
    return;
  }

  unsigned char index = atoi(str);
  dumpCurveByIndex(sender->GetSerial(), index);
}

/** Set pattern of week for curve given by index .
  Input: index pattern
  Output: nothing
*/
void cmd_setCurvePattern(SerialCommands* sender)
{
  char* str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "setCurvePattern without curve index");
    return;
  }

  unsigned char index = atoi(str);

  str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "setCurvePattern without pattern");
    return;
  }

  unsigned char pattern = atoi(str);

  TCurve* c = getCurve(index);
  if (c == NULL)
  {
    error(sender->GetSerial(), "setCurvePattern for invalid index");
    return;
  }

  c->setWeekPattern(pattern);

  dumpCurveByIndex(sender->GetSerial(), index);
}

/** Set all setpoints for curve given by index .
  Input: index (h m t)* /
  Output: nothing
*/
void cmd_setCurvePoints (SerialCommands* sender)
{
  char* str = sender->Next();
  if (str == NULL)
  {
    error(sender->GetSerial(), "setCurvePoints without curve index");
    return;
  }

  unsigned char index = atoi(str);
  TCurve* c = getCurve(index);
  if (c == NULL)
  {
    error(sender->GetSerial(), "No matching curve");
    return;
  }

  parseAndSetCurvePoints(c, sender);
}
SerialCommand commands[] = {
  SerialCommand("getMode", cmd_getMode),
  SerialCommand("OFF", cmd_off),
  SerialCommand("AUTO", cmd_auto),
  SerialCommand("MANU", cmd_manu),
  SerialCommand("getTime", cmd_getTime),
  SerialCommand("setTime", cmd_setTime),
  SerialCommand("getDate", cmd_getDate),
  SerialCommand("setDate", cmd_setDate),
  SerialCommand("getSetpoint", cmd_getSetpoint),
  SerialCommand("getTemperature", cmd_getTemperature),
  SerialCommand("getNbCurves", cmd_getNbCurves),
  SerialCommand("delCurves", cmd_delCurves),
  SerialCommand("delCurve", cmd_delCurve),
  SerialCommand("addCurve", cmd_addCurve),
  SerialCommand("getCurve", cmd_getCurve),
  SerialCommand("setCurvePattern", cmd_setCurvePattern),
  SerialCommand("setCurvePoints", cmd_setCurvePoints),
};

SerialCommand commandsBT[] = {
  SerialCommand("getMode", cmd_getMode),
  SerialCommand("OFF", cmd_off),
  SerialCommand("AUTO", cmd_auto),
  SerialCommand("MANU", cmd_manu),
  SerialCommand("getTime", cmd_getTime),
  SerialCommand("setTime", cmd_setTime),
  SerialCommand("getDate", cmd_getDate),
  SerialCommand("setDate", cmd_setDate),
  SerialCommand("getSetpoint", cmd_getSetpoint),
  SerialCommand("getTemperature", cmd_getTemperature),
  SerialCommand("getNbCurves", cmd_getNbCurves),
  SerialCommand("delCurves", cmd_delCurves),
  SerialCommand("delCurve", cmd_delCurve),
  SerialCommand("addCurve", cmd_addCurve),
  SerialCommand("getCurve", cmd_getCurve),
  SerialCommand("setCurvePattern", cmd_setCurvePattern),
  SerialCommand("setCurvePoints", cmd_setCurvePoints),
};

void installCommands()
{
  serial_commands_.SetDefaultHandler(cmd_unrecognized);
  serial1_commands_.SetDefaultHandler(cmd_unrecognized);

  for (char i = 0; i < sizeof(commandsBT) / sizeof(SerialCommand); i++)
  {
    serial_commands_.AddCommand(&commands[i]);
    serial1_commands_.AddCommand(&commandsBT[i]);
  }
}

void processCommands()
{
  if (Serial.available())
  {
    serial_commands_.ReadSerial();
  }

  if (Serial1.available())
  {
    serial1_commands_.ReadSerial();
  }
}

