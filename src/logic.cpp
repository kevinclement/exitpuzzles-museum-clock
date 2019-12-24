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

void Logic::resetHand(bool hour) {
  if (hour) {
    stepmotor._resetHour = !stepmotor._resetHour;
    stepmotor.setSpeed(true, 20);
  } else {
    stepmotor._resetMinute = !stepmotor._resetMinute;
    stepmotor.setSpeed(false, 20);
  }
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
    _fresh = false;
    stepmotor.hour_stepper = hour.position / 2;
    _hourPos = hour.position;
  }

  if (_minPos != minute.position) {
    _fresh = false;
    stepmotor.minute_stepper = minute.position / 2;
    _minPos = minute.position;
  }

  // if this is a clean boot and we've solved it then reset the motors
  if (_fresh && hourSensor.solved && minuteSensor.solved) {
    serial.print("detected boot with solved position still.  resetting hands...%s", CRLF);
    resetHand(true);
    resetHand(false);
    _fresh = false;
    return;
  }

  // if we're resetting, the don't trigger solution
  if (stepmotor._resetHour || stepmotor._resetMinute) {
    if (_hrt != 0 && millis() - _hrt > 800) {
      serial.print("ran hour long enough. stopping now.%s", CRLF);
      stepmotor._resetHour = false;
      hour.encoder.clearCount();
      _hrt = 0;
    }

    if (stepmotor._resetHour && hourSensor.solved && _hrt == 0) {
      serial.print("found hour home. starting timer.%s", CRLF);
      stepmotor.setSpeed(true, 100);
      _hrt = millis();
    }

    if (_mrt != 0 && millis() - _mrt > 3000) {
      serial.print("ran minute long enough. stopping now.%s", CRLF);
      stepmotor._resetMinute = false;
      minute.encoder.clearCount();
      _mrt = 0;
    }

    if (stepmotor._resetMinute && minuteSensor.solved && _mrt == 0) {
      serial.print("found minute home. starting timer.%s", CRLF);
      stepmotor.setSpeed(false, 100);
      _mrt = millis();
    }
  } else {
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

      

      

