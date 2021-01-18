#pragma once

#include "Arduino.h"
#include <AccelStepper.h>

class Logic;

class Stepper {
  public:
    Stepper(Logic &logic);
    void setup();
    void handle();
    void setSpeed(bool hour, int speed);
    void kevintest();

    bool _enabled      = false; // default to off until we get the motor whine under control
    bool _resetHour    = false;
    bool _resetMinute  = false;

    int hour_stepper   = 0;
    int minute_stepper = 0;
    
  private:
    Logic &_logic;
    
};
