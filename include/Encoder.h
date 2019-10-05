#pragma once

#include "Arduino.h"
#include <ESP32Encoder.h>

class Logic;

class Encoder {
  public:
    Encoder(Logic &logic);
    void setup(const char * label, int PIN1, int PIN2, int MAX_VALUE);
    void handle();

    int time = 0;
    int position = 0;
    bool debug = false;

  private:
    Logic &_logic;
    const char * _label;
    int _MAX_VALUE;

    int curPos = 0;
    unsigned long lastEnc  = millis();

    ESP32Encoder encoder;
    void setValue(int);
};
