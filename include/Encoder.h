#pragma once

#include "Arduino.h"
#include <ESP32Encoder.h>

class Logic;

class Encoder {
  public:
    Encoder(Logic &logic);
    void setup(const char * label, int PIN1, int PIN2);
    void handle();
    void pause();
    void clear();
    void status();

    bool disabled = false;
    int position = 0;
    bool debug = false;
    ESP32Encoder encoder;

  private:
    Logic &_logic;
    const char * _label;
    unsigned long lastEnc  = millis();
};
