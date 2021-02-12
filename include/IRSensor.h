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
    bool solved = false;
    bool debug = false; 
    
  private:
    Logic &_logic;
    uint _pin;
    const char * _label;

    int readSensor();

    // unsigned long _seen_solve_bit_at = 0;
    // bool fakeIt = false;
};