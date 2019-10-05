#pragma once

#include "Arduino.h"

class Logic;

class HallSensor {
  public:
    HallSensor(Logic &logic, uint PIN, const char * label);
    void setup();
    void handle();
    void fake();

    bool solved = false;
    bool debug = false; 

  private:
    Logic &_logic;
    uint _pin;
    const char * _label;

    int readSensor();

    unsigned long _seen_solve_bit_at = 0;
    bool fakeIt = false;
};