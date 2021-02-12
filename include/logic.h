#pragma once

#include "SerialManager.h"
#include "Stepper.h"
#include "Encoder.h"
#include "Magnet.h"
#include "IRSensor.h"

class Logic {
public:
  Logic();
  SerialManager serial;  
  Stepper stepmotor;
  Encoder hour;
  Encoder minute;
  Magnet magnet;
  IRSensor leftSensor;
  IRSensor rightSensor;

  void setup();
  void handle();
  void solved();
  void status();
  void resetHand(bool hour);

private:

  int _hourPos = 0;
  int _minPos = 0;
  bool _hs = false;
  bool _ms = false;
  bool _solved = false;
  bool _fresh = true;
  unsigned long _hrt = 0;
  unsigned long _mrt = 0;
};

