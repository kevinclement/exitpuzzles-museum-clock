#include "Arduino.h"
#include "logic.h"
#include "consts.h"

#define HOUR_ENC_1_PIN    21
#define HOUR_ENC_2_PIN    17
#define MINUTE_ENC_1_PIN  25
#define MINUTE_ENC_2_PIN  26

#define PIN_SENSOR_LEFT 4
#define PIN_SENSOR_RIGHT 5

Logic::Logic() 
  : serial(),
    stepmotor(*this),
    hour(*this),
    minute(*this),
    magnet(*this),
    leftSensor(*this, PIN_SENSOR_LEFT, "LEFT"),
    rightSensor(*this, PIN_SENSOR_RIGHT, "RIGHT")
{
}

void Logic::setup() {
  serial.setup("");

  // pedastal rotary encoders
  hour.setup("hour", HOUR_ENC_1_PIN, HOUR_ENC_2_PIN);
  minute.setup("minute", MINUTE_ENC_1_PIN, MINUTE_ENC_2_PIN);

  // vidstepper
  stepmotor.setup();

  // magnet
  magnet.setup();

  // IR sensors
  leftSensor.setup();
  rightSensor.setup();
}

void Logic::solved() {
  serial.print("Solved!\n");
  _solved = true;
  magnet._enabled = false;
  stepmotor._enabled = false;
  status();
}

void Logic::resetHand(bool hour) {
  // if (hour) {
  //   stepmotor._resetHour = !stepmotor._resetHour;
  //   stepmotor.setSpeed(true, 20);
  // } else {
  //   stepmotor._resetMinute = !stepmotor._resetMinute;
  //   stepmotor.setSpeed(false, 20);
  // }
}

void Logic::handle() {
  serial.handle();
  magnet.handle();
  hour.handle();
  minute.handle();
  stepmotor.handle();
  leftSensor.handle();
  rightSensor.handle();

  // ## IR Sensor Logic #################################

  // NOTE: since I'm using debounce and watching for fell, it should
  //   handle the case where I startup with hands already over sensor
  //   since that won't be a 'fall' event.  It will then move the hands
  //   and watch for it to fall again.
  if (rightSensor.sensor.fell()) {
    // TODO: better and status
    Serial.println("right side: ON!!");

    // TODO: add back when motors
    // if (ms.state == RESETTING) {
    //   ms.state = FOUND_SENSOR;
    //   ms.move(-124);
    // }
  }

  if (leftSensor.sensor.fell()) {
    // TODO: better and status
    Serial.println("left side: ON!!");

    // TODO: add back when motors
    // if (hs.state == RESETTING) {
    //   hs.state = FOUND_SENSOR;
    //   hs.move(124);
    // }  
  }

  // ####################################################

  // if (_hourPos != hour.position) {
  //   _fresh = false;
  //   stepmotor.hour_stepper = hour.position / 2;
  //   _hourPos = hour.position;
  // }

  // if (_minPos != minute.position) {
  //   _fresh = false;
  //   stepmotor.minute_stepper = minute.position / 2;
  //   _minPos = minute.position;
  // }

  // if this is a clean boot and we've solved it then reset the motors
  // if (_fresh && hourSensor.solved && minuteSensor.solved) {
  //   serial.print("detected boot with solved position still.  resetting hands...%s", CRLF);
  //   resetHand(true);
  //   resetHand(false);
  //   _fresh = false;
  //   return;
  // }

  // if we're resetting, the don't trigger solution
  // if (stepmotor._resetHour || stepmotor._resetMinute) {
  //   if (_hrt != 0 && millis() - _hrt > 1000) {
  //     serial.print("ran hour long enough. stopping now.%s", CRLF);
  //     stepmotor._resetHour = false;
  //     hour.encoder.clearCount();
  //     _hrt = 0;
  //   }

  //   if (stepmotor._resetHour && hourSensor.solved && _hrt == 0) {
  //     serial.print("found hour home. starting timer.%s", CRLF);
  //     stepmotor.setSpeed(true, 100);
  //     _hrt = millis();
  //   }

  //   if (_mrt != 0 && millis() - _mrt > 3050) {
  //     serial.print("ran minute long enough. stopping now.%s", CRLF);
  //     stepmotor._resetMinute = false;
  //     minute.encoder.clearCount();
  //     _mrt = 0;
  //   }

  //   if (stepmotor._resetMinute && minuteSensor.solved && _mrt == 0) {
  //     serial.print("found minute home. starting timer.%s", CRLF);
  //     stepmotor.setSpeed(false, 100);
  //     _mrt = millis();
  //   }
  // } else {
  //   if (hourSensor.solved != _hs || minuteSensor.solved != _ms) {
  //     _hs = hourSensor.solved;
  //     _ms = minuteSensor.solved;

  //     if (!_solved && _hs && _ms) {
  //       solved();
  //     } else {
  //       status();
  //     }
  //   }
  // }
  
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
      "encDisabled:%s,"
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
      minute.disabled ? "true" : "false",
      _hs ? "true" : "false",
      _ms ? "true" : "false",
      stepmotor._enabled ? "true" : "false",
      _solved ? "true" : "false",

      CRLF);

  serial.print(cMsg);
}
