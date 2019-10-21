#include "Arduino.h"
#include "logic.h"
#include "consts.h"

#define HOUR_ENC_1_PIN    4
#define HOUR_ENC_2_PIN    36
#define MINUTE_ENC_1_PIN  25
#define MINUTE_ENC_2_PIN  26

Logic::Logic() 
  : serial(),
    stepmotor(*this),
    hour(*this),
    minute(*this),
    magnet(*this),
    hourSensor(*this,   A3, "HOUR"),
    minuteSensor(*this, A2, "MINUTE")
{
}

void Logic::setup() {
  serial.setup("");
  hour.setup("hour", HOUR_ENC_1_PIN, HOUR_ENC_2_PIN, 12);
  minute.setup("minute", MINUTE_ENC_1_PIN, MINUTE_ENC_2_PIN, 60);
  stepmotor.setup();
  magnet.setup();
  hourSensor.setup();
  minuteSensor.setup();
}

void Logic::solved() {
  serial.print("Solved!\n");
  _solved = true;
  magnet._enabled = false;
  stepmotor._enabled = false;
  status();
}

void Logic::handle() {
  serial.handle();
  magnet.handle();
  hour.handle();
  minute.handle();
  stepmotor.handle();
  hourSensor.handle();
  minuteSensor.handle();

  if (_hourPos != hour.position) {
    stepmotor.hour_stepper = hour.position / 2;
    _hourPos = hour.position;
  }

  if (_minPos != minute.position) {
    stepmotor.minute_stepper = minute.position / 2;
    _minPos = minute.position;
  }

  if (hourSensor.solved != _hs || minuteSensor.solved != _ms) {
    _hs = hourSensor.solved;
    _ms = minuteSensor.solved;

    if (!_solved && _hs && _ms) {
      solved();
    } else {
      status();
    }
  }
}

void Logic::status() {
  char cMsg[254];

  sprintf(cMsg, 
    "status="
      "version:%s,"
      "gitDate:%s,"
      "buildDate:%s,"

      "encHour:%d,"
      "encMinute:%d,"
      "hs:%s,"
      "ms:%s,"
      "stepper:%s,"
      "solved:%s"

      "%s"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,

      hour.encoder.getCount(),
      minute.encoder.getCount(),
      _hs ? "true" : "false",
      _ms ? "true" : "false",
      stepmotor._enabled ? "true" : "false",
      _solved ? "true" : "false",

      CRLF);

  serial.print(cMsg);
}

      

      

