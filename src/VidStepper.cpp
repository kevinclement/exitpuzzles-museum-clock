#include "Arduino.h"
#include "VidStepper.h"

VidStepper::VidStepper(AccelStepper stepper)
: _stepper(stepper)
{
}
 
void VidStepper::setup(float maxSpeed, float solve) {
  _stepper.setSpeed(300);  
  _stepper.setMaxSpeed(maxSpeed);

  // setup solve boundaries to provide some wiggle room
  _solveMin = solve - (2 * STEP_SIZE);
  _solveMax = solve + (2 * STEP_SIZE); 
  
  state = IDLE;
}

void VidStepper::handle() {
  _stepper.runSpeedToPosition();

  if (state == FOUND_SENSOR && distanceToGo() == 0) {
    state = RESET;
    _stepper.setCurrentPosition(0);
  }

  if (state == NUDGING && distanceToGo() == 0) {
    state = NUDGED;
  }

  if (state == GAMEON && distanceToGo() == 0) {
    float pos = abs();
    if (pos >= _solveMin && pos <= _solveMax) {
      if (!solved) {
        // TODO: better output
        Serial.println("!! SOLVED !!");
      }
      solved = true;
    } else {
      solved = false;
    }
  } 
}

long VidStepper::abs() {
  // always report the position in absolute positive number
  long abs = _stepper.currentPosition() % NUM_STEPS;
  if (abs < 0) {
    // looks weird, but + is because number is already negative
    abs = NUM_STEPS + abs;
  }

  return abs;
}

void VidStepper::move(long relative) {
  _stepper.move(relative);
  _stepper.setSpeed(300);
}

void VidStepper::nudge() {
  state = NUDGING;
  move(50);
}

void VidStepper::reset() {
  state = RESETTING;
  move(NUM_STEPS);
}

long VidStepper::distanceToGo() {
  return _stepper.distanceToGo();
}

void VidStepper::status() {
  Serial.print("  current: ");
  Serial.print(_stepper.currentPosition());
  Serial.print(" target: ");
  Serial.print(_stepper.targetPosition());
  Serial.print(" mod: ");
  Serial.print(_stepper.currentPosition() % NUM_STEPS);
  Serial.print(" abs: ");
  Serial.print(abs());
  Serial.print(" min: ");
  Serial.print(_solveMin);
  Serial.print(" max: ");
  Serial.print(_solveMax);  

  if (solved) {
    Serial.print(" SOLVED");
  }
}