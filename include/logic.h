#pragma once

#include "SerialManager.h"
#include "Encoder.h"
#include "Magnet.h"
#include "IRSensor.h"
#include "VidStepper.h"
#include "Audio.h"

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
  Audio audio;

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
  bool _finished = false;
  unsigned long solvedAt = 0;
};

