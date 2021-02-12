#pragma once

#include "Arduino.h"
#include <Bounce2.h>

class Logic;

class IRSensor {
  public:
    IRSensor(Logic &logic, uint PIN, const char * label);
    void setup();
    void handle();

    Bounce sensor = Bounce();
    
  private:
    Logic &_logic;
    uint _pin;
    const char * _label;
};