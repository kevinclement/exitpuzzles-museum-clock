#pragma once

#include "Arduino.h"

class Logic;

class Encoder {
  public:
    Encoder(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
    
};
