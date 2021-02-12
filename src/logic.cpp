#include "Arduino.h"
#include "logic.h"
#include "consts.h"

#define HOUR_ENC_1_PIN    21
#define HOUR_ENC_2_PIN    17
#define MINUTE_ENC_1_PIN  25
#define MINUTE_ENC_2_PIN  26

#define HOUR_MTR_PIN1     12
#define HOUR_MTR_PIN2     27
#define HOUR_MTR_PIN3     33
#define HOUR_MTR_PIN4     15

#define MIN_MTR_PIN1      32
#define MIN_MTR_PIN2      14
#define MIN_MTR_PIN3      22
#define MIN_MTR_PIN4      23

#define PIN_SENSOR_LEFT 4
#define PIN_SENSOR_RIGHT 5

Logic::Logic() 
  : serial(),
    hourMotor(AccelStepper(AccelStepper::FULL4WIRE, HOUR_MTR_PIN1, HOUR_MTR_PIN2, HOUR_MTR_PIN3, HOUR_MTR_PIN4)),
    minuteMotor(AccelStepper(AccelStepper::FULL4WIRE, MIN_MTR_PIN1, MIN_MTR_PIN2, MIN_MTR_PIN3, MIN_MTR_PIN4)),
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
  hourMotor.setup(800, 180);
  minuteMotor.setup(1200, 540);
  
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
  hourMotor.handle();
  minuteMotor.handle();
  leftSensor.handle();
  rightSensor.handle();

  // ## IR Sensor Logic #################################

  // NOTE: since I'm using debounce and watching for fell, it should
  //   handle the case where I startup with hands already over sensor
  //   since that won't be a 'fall' event.  It will then move the hands
  //   and watch for it to fall again.
  if (rightSensor.sensor.fell()) {
    // TODO: better and status
    //_logic.serial.print("%s: hall turned off\r\n", _label);
    //   _logic.serial.print("%s: hall turned on\r\n", _label);
    Serial.println("right side: ON!!");

    if (minuteMotor.state == RESETTING) {
      minuteMotor.state = FOUND_SENSOR;
      minuteMotor.move(-124);
    }
  }

  if (leftSensor.sensor.fell()) {
    // TODO: better and status
    Serial.println("left side: ON!!");

    if (hourMotor.state == RESETTING) {
      hourMotor.state = FOUND_SENSOR;
      hourMotor.move(124);
    }  
  }

  // ####################################################

  // ## MOTORS ##############################
  if (hourMotor.state == RESET && minuteMotor.state == IDLE) {
    hourMotor.reset();
  }

  if (hourMotor.state == RESETTING && hourMotor.distanceToGo() == 0) {
    // TODO: proper serial
    Serial.println("WARN: did full circle with hour hand and didn't trigger reset.");
    Serial.println("Moving minute hand and will try again.");
    
    hourMotor.state = IDLE;
    minuteMotor.nudge();
  }

  if (minuteMotor.state == NUDGED) {
    // TODO: proper serial
    Serial.println("Finished nudging minute hand.  Resetting hour again.");
    minuteMotor.state = IDLE;
    hourMotor.reset();
  }

  if (hourMotor.state == RESET && minuteMotor.state == RESET) {
    hourMotor.state = GAMEON;
    minuteMotor.state = GAMEON;
  }

  // ########################################


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
      true ? "true" : "false",
      _solved ? "true" : "false",

      CRLF);

  // TODO: proper stepper enabled
  serial.print(cMsg);
}
