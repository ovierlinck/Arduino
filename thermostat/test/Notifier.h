#ifndef NOTIFIER_H
#define NOTIFIER_H

#include "Controller.h"

void onTemperature(unsigned char temp);
void onSetpoint(unsigned char temp);
void onCommand(bool command);
void onMode(Mode mode);
void onDateTime(uint8_t day, uint8_t month, uint8_t year, uint8_t hours, uint8_t minutes, uint8_t seconds);
    
#endif
