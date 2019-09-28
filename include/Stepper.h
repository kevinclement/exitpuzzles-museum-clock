#pragma once

#include "Arduino.h"
#include <AccelStepper.h>

class Logic;

class Stepper {
  public:
    Stepper(Logic &logic);
    void setup();
    void handle();
    
    bool _enabled = true;
  private:
    Logic &_logic;
    
};
