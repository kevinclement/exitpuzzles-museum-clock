#pragma once

#include "SerialManager.h"
#include "Encoder.h"
#include "Magnet.h"
#include "IRSensor.h"
#include "VidStepper.h"

class Logic {
public:
  Logic();
  SerialManager serial;
  VidStepper hourMotor;
  VidStepper minuteMotor;
  Encoder hour;
  Encoder minute;
  Magnet magnet;
  IRSensor leftSensor;
  IRSensor rightSensor;

  void setup();
  void handle();
  void solved();
  void status();
  void resetHands();

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

