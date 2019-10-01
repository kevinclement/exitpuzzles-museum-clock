#pragma once

#include "SerialManager.h"
#include "Stepper.h"
#include "Encoder.h"
#include "WifiUpdate.h"
#include "Magnet.h"
#include "HallSensor.h"

class Logic {
public:
  Logic();
  SerialManager serial;  
  Stepper stepmotor;
  Encoder encoder;
  WifiUpdate wifi;
  Magnet magnet;
  HallSensor hourSensor;
  HallSensor minuteSensor;

  void setup();
  void handle();
  void solved();
  void status();

private:

  bool _solved = false;
  bool _hs = false;
  bool _ms = false;
};

