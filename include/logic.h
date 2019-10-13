#pragma once

#include "SerialManager.h"
#include "Stepper.h"
#include "Encoder.h"
#include "Magnet.h"
#include "HallSensor.h"

class Logic {
public:
  Logic();
  SerialManager serial;  
  Stepper stepmotor;
  Encoder hour;
  Encoder minute;
  Magnet magnet;
  HallSensor hourSensor;
  HallSensor minuteSensor;

  void setup();
  void handle();
  void solved();
  void status();

private:

  int _hourPos = 0;
  int _minPos = 0;
  bool _hs = false;
  bool _ms = false;
  bool _solved = false;
};

