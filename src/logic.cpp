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

#define SOLVED_WAIT_MS 2500

Logic::Logic() 
  : serial(),
    hourMotor(*this, AccelStepper(AccelStepper::FULL4WIRE, HOUR_MTR_PIN1, HOUR_MTR_PIN2, HOUR_MTR_PIN3, HOUR_MTR_PIN4)),
    minuteMotor(*this, AccelStepper(AccelStepper::FULL4WIRE, MIN_MTR_PIN1, MIN_MTR_PIN2, MIN_MTR_PIN3, MIN_MTR_PIN4)),
    hour(*this),
    minute(*this),
    magnet(*this),
    leftSensor(*this, PIN_SENSOR_LEFT, "LEFT"),
    rightSensor(*this, PIN_SENSOR_RIGHT, "RIGHT"),
    audio(*this)
{
}

void Logic::setup() {
  serial.setup("");
  audio.setup();

  // pedastal rotary encoders
  hour.setup("hour", HOUR_ENC_1_PIN, HOUR_ENC_2_PIN);
  minute.setup("minute", MINUTE_ENC_1_PIN, MINUTE_ENC_2_PIN);

  // vidstepper
  hourMotor.setup("HOUR", 800, 535);
  minuteMotor.setup("MINUTE", 1200, 180);
  
  // magnet
  magnet.setup();

  // IR sensors
  leftSensor.setup();
  rightSensor.setup();

  // reset motor positions, start with hour hand
  hourMotor.reset();
}

void Logic::solved() {
  serial.print("Solved!\n");
  _solved = true;
  audio.playTone();
  magnet._enabled = false;
  status();

  // add small delay here so it doesn't open exactly when audio plays
  delay(200);
}

void Logic::resetHands() {
  minuteMotor.state = IDLE;
  hourMotor.reset();
}

void Logic::handle() {
  serial.handle();
  magnet.handle();
  hour.handle();
  minute.handle();  
  leftSensor.handle();
  rightSensor.handle();
  audio.handle();

  // ## IR Sensor Logic #################################

  // NOTE: since I'm using debounce and watching for fell, it should
  //   handle the case where I startup with hands already over sensor
  //   since that won't be a 'fall' event.  It will then move the hands
  //   and watch for it to fall again.
  if (rightSensor.sensor.fell()) {
    serial.print("RIGHT: IR turned on\r\n");

    if (minuteMotor.state == RESETTING) {
      minuteMotor.state = FOUND_SENSOR;
      minuteMotor.move(-124);
    }

    status();
  }
  else if (rightSensor.sensor.rose()) {
    serial.print("RIGHT: IR turned off\r\n");
    status();
  }

  if (leftSensor.sensor.fell()) {
    serial.print("LEFT: IR turned on\r\n");

    if (hourMotor.state == RESETTING) {
      hourMotor.state = FOUND_SENSOR;
      hourMotor.move(124);
    }

    status();
  } 
  else if (leftSensor.sensor.rose()) {
    serial.print("LEFT: IR turned off\r\n");
    status();
  }
  // ####################################################

  // ## MOTORS ##########################################
  if (hourMotor.state == RESET && minuteMotor.state == IDLE) {
    minuteMotor.reset();
  }

  if (hourMotor.state == RESETTING && hourMotor.distanceToGo() == 0) {
    serial.print("WARN: did full circle with hour hand and didn't trigger reset.%s", CRLF);
    serial.print("Moving minute hand and will try again.%s", CRLF);

    hourMotor.state = IDLE;
    minuteMotor.nudge();
  }

  if (minuteMotor.state == NUDGED) {
    serial.print("Finished nudging minute hand.  Resetting hour again.%s", CRLF);
    minuteMotor.state = IDLE;
    hourMotor.reset();
  }

  hourMotor.handle();
  minuteMotor.handle();
  
  if (_hourMotorPos != hourMotor.position) {
    _hourMotorPos = hourMotor.position;

    // Update the status once motor is done moving
    if (hourMotor.distanceToGo() == 0) {
      status();
    }
  }
  if (_minMotorPos != minuteMotor.position) {
    _minMotorPos = minuteMotor.position;

    // Update the status once motor is done moving
    //   NOTE: might want to remove this check to get most current values
    //   don't know if that is too excessive though
    if (minuteMotor.distanceToGo() == 0) {
      status();
    }
  }

  if (hourMotor.state == RESET && minuteMotor.state == RESET) {
    hourMotor.state = GAMEON;
    minuteMotor.state = GAMEON;

    // reset internal state 
    _hourPos = _minPos = _hourMotorPos = _minMotorPos = _solvedAt = 0;    
    _solved = false;

    // reset encoder internal state
    hour.clear();
    minute.clear();

    status();

    // move minute hand a bit so its not totally overlapped to start
    minute.encoder.setCount(10);
  }
  // ####################################################

  // ## Rotary Encoders #################################
  if (_hourPos != hour.position) {
    int delta = _hourPos - hour.position;
    if (delta > 0) {
      hourMotor.move(-STEP_SIZE);
    } else if (delta < 0) {
      hourMotor.move(STEP_SIZE);
    }

    _hourPos = hour.position;
  }

  if (_minPos != minute.position) {   
    int delta = _minPos - minute.position;
    if (delta > 0) {
      minuteMotor.move(-STEP_SIZE);
    } else if (delta < 0) {     
      minuteMotor.move(STEP_SIZE);
    }

    _minPos = minute.position;
  }
  // ##################################################

  // ## Final Solved Check ############################
  if (minuteMotor.solved && hourMotor.solved) {
    if (_solvedAt == 0) {
      serial.print("initial solve.  waiting for timeout...\r\n");
      _solvedAt = millis();
    }

    if (!_solved && millis() - _solvedAt > SOLVED_WAIT_MS) {
      solved();
    } 
  } else {
    _solvedAt = 0;
  }
  // ##################################################
  
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
      "encoder:%s,"
      "hs:%s,"
      "ms:%s,"
      "hourMotor:%ld,"
      "minuteMotor:%ld,"
      "solved:%s"

      "%s"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,

      hour.encoder.getCount(),
      minute.encoder.getCount(),
      minute.disabled ? "false" : "true",
      hourMotor.solved ? "true" : "false",
      minuteMotor.solved ? "true" : "false",
      _hourMotorPos,
      _minMotorPos,
      _solved ? "true" : "false",

      CRLF);

  serial.print(cMsg);
}
