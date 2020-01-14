/* List of Controller functions available to tools (typically, to Commands.cpp!)*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

class TCurve;

enum Mode {OFF, AUTO, MANUAL};

void setMode(Mode mode);
Mode getMode();
const char* getModeName(Mode mode);

void setSetpoint(unsigned char pSetpoint);
unsigned char getSetpoint();

void setDate(unsigned char days, unsigned char months, unsigned int year, unsigned char dayOfWeek);
/** true if date available, false otherwise*/
bool getDate(unsigned char& days, unsigned char& months, unsigned int& year, unsigned char& dayOfWeek);

void setTime(unsigned char hours, unsigned char minutes);
/** true if time available, false otherwise*/
bool getTime(unsigned char& hours, unsigned char& minutes);

float getTemperature();

/** Program Management */
unsigned char getNbCurves();
void deleteAllCurves();
void deleteCurve(unsigned char index);
unsigned char addCurve(TCurve curve);
TCurve* getCurve(unsigned char index);

#endif
