#ifndef TPROGRAM_H
#define TPROGRAM_H

const char TPROGRAM_LENGTH_NAME = 32;
const char TPROGRAM_MAX_CURVES = 5;

#include "TCurve.h"

class TProgram
{
  public:
    TProgram();
    TProgram(const char* name);

    void setName(const char* name);
    const char* getName();

    bool addCurve(TCurve& curve);
    void removeAllCurves();
    char getNbCurves();
    TCurve* getCurve(char index);

    char getSetpoint(char day, char h, char m);

    /** fill the given array of curves, one per day of the week, 1 curve per day, applicable for taht day. One curve may be present for multiple days*/
    void getWeekCurves(TCurve* curves[7]);

    TCurve* getCurveForDay(char day);

  private:


    char _name[TPROGRAM_LENGTH_NAME];
    TCurve _curves[TPROGRAM_MAX_CURVES];
    char _nbCurves = 0;
};

#endif
