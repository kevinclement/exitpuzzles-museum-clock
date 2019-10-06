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

void Stepper::setup() {
  // setup enabled pin for motors (shared)
  pinMode(ENABLE_PIN, OUTPUT);

  // Setup max speed for steppers
  stepperHour.setMaxSpeed(500);
  stepperMinute.setMaxSpeed(500);

}

void Stepper::handle() {
  // TODO: add this back once I debug why it doesn't work when doing encoder increments/decrements
  // if (!_enabled || (stepperHour.distanceToGo() == 0 && stepperMinute.distanceToGo() == 0)) {
  //   digitalWrite(ENABLE_PIN, HIGH);
  // } else {
    digitalWrite(ENABLE_PIN, LOW);
  // }

  stepperHour.moveTo(hour_stepper);
  stepperHour.setSpeed(100);
  stepperHour.runSpeedToPosition();

  stepperMinute.moveTo(minute_stepper);
  stepperMinute.setSpeed(100);
  stepperMinute.runSpeedToPosition();
}
