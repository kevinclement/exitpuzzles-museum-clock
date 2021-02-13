#pragma once

#include "Arduino.h"
#include <Tone32.h>

#define BUZZER_PIN 16
#define BUZZER_CHANNEL 0

class Logic;

class Audio {
  public:
    Audio(Logic &logic);
    void setup();
    void handle();

    bool _playTone = false;

  private:
    Logic &_logic;

};
