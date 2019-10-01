#pragma once

#include "Arduino.h"

class Logic;

class HallSensor {
  public:
    HallSensor(Logic &logic, uint PIN);
    void setup();
    void handle();

  private:
    Logic &_logic;
    uint _pin;

};
