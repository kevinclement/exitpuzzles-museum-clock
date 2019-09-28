#pragma once

#include "Arduino.h"
#include <ESP32Encoder.h>

class Logic;

class Encoder {
  public:
    Encoder(Logic &logic);
    void setup();
    void handle();

    int HOUR = 0;
    int MINUTE = 0;

  private:
    Logic &_logic;

    void setHour(int);
    void setMinute(int);
    
};
