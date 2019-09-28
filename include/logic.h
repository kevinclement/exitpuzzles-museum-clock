#pragma once

#include "SerialManager.h"
#include "Stepper.h"
#include "Encoder.h"
#include "WifiUpdate.h"
#include "Magnet.h"

class Logic {
public:
  Logic();
  SerialManager serial;  
  Stepper stepmotor;
  Encoder encoder;
  WifiUpdate wifi;
  Magnet magnet;

  void setup();
  void handle();
  void solved();
  void status();

private:

};

