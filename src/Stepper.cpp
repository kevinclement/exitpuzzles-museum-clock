#include "Arduino.h"
#include "Stepper.h"
#include "logic.h"

#define ENABLE_PIN        14
#define HOUR_DIR_PIN      15
#define HOUR_STEP_PIN     32
#define MINUTE_DIR_PIN    27
#define MINUTE_STEP_PIN   33

AccelStepper stepperHour(1, HOUR_STEP_PIN, HOUR_DIR_PIN);
AccelStepper stepperMinute(1, MINUTE_STEP_PIN, MINUTE_DIR_PIN);

Stepper::Stepper(Logic &logic)
: _logic(logic)
{  
}

void Stepper::setSpeed(bool hour, int speed) {
  if (hour) {
    stepperHour.setSpeed(speed);
  }
  else {
    stepperMinute.setSpeed(speed);
  }
}

void Stepper::setup() {
  // setup enabled pin for motors (shared)
  pinMode(ENABLE_PIN, OUTPUT);

  // Setup max speed for steppers
  stepperHour.setMaxSpeed(200);
  stepperMinute.setMaxSpeed(200);

  // TMP #################################################
  // stepperMinute.setMaxSpeed(1000);
  // stepperMinute.setSpeed(50);	

  // stepperHour.setMaxSpeed(1000);
  // stepperHour.setSpeed(50);	
  //stepperHour.setMaxSpeed(100);
  //stepperHour.setAcceleration(0);
  // stepperHour.moveTo(500);
  // digitalWrite(ENABLE_PIN, true ? LOW : HIGH);
  // ####################################################
}

void Stepper::kevintest() {

  // if (stepperHour.distanceToGo() == 0)
  //     stepperHour.moveTo(-stepperHour.currentPosition());

  // stepperHour.run();
  // stepperHour.runSpeed();
  stepperMinute.runSpeed();
}

void Stepper::handle() {
  // TODO: add this back once I debug why it doesn't work when doing encoder increments/decrements
  // if (!_enabled || (stepperHour.distanceToGo() == 0 && stepperMinute.distanceToGo() == 0)) {
  //   digitalWrite(ENABLE_PIN, HIGH);
  // } else {
    // digitalWrite(ENABLE_PIN, LOW);
  // }

  digitalWrite(ENABLE_PIN, _enabled || _resetHour || _resetMinute ? LOW : HIGH);

  if (_resetHour) {
    stepperHour.runSpeed();
  } 
  
  if (_resetMinute) {
    stepperMinute.runSpeed();
  } 
  
  if (!_resetHour && !_resetMinute) {
    stepperHour.moveTo(hour_stepper);
    stepperHour.setSpeed(100);
    stepperHour.runSpeedToPosition();

    stepperMinute.moveTo(minute_stepper);
    stepperMinute.setSpeed(100);
    stepperMinute.runSpeedToPosition();
  }
}
