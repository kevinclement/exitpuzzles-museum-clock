#include "Arduino.h"
#include "VidStepper.h"
#include "logic.h"

VidStepper::VidStepper(Logic &logic, AccelStepper stepper)
: _logic(logic), 
  _stepper(stepper)
{
}
 
void VidStepper::setup(const char * label, float maxSpeed, float solve) {
  _stepper.setSpeed(300);  
  _stepper.setMaxSpeed(maxSpeed);

  // setup solve boundaries to provide some wiggle room
  _solveMin = solve - (2 * STEP_SIZE);
  _solveMax = solve + (2 * STEP_SIZE); 
  
  state = IDLE;
  _label = label;
}

void VidStepper::handle() {
  _stepper.runSpeedToPosition();
  calcPosition();

  if (state == FOUND_SENSOR && distanceToGo() == 0) {
    state = RESET;
    _stepper.setCurrentPosition(0);
  }

  if (state == NUDGING && distanceToGo() == 0) {
    state = NUDGED;
  }

  if (state == GAMEON && distanceToGo() == 0) {
    if (position >= _solveMin && position <= _solveMax) {
      if (!solved) {
        _logic.serial.print("!! %s: SOLVED !!\r\n", _label);
      }
      solved = true;
    } else {
      solved = false;
    }
  } 
}

void VidStepper::calcPosition() {
  // always report the position in absolute positive number
  position = _stepper.currentPosition() % NUM_STEPS;
  if (position < 0) {
    // looks weird, but + is because number is already negative
    position = NUM_STEPS + position;
  }
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
  _logic.serial.print("%-8s: current: %d target: %d mod: %d pos: %d min: %d max: %d ", 
    _label,
    _stepper.currentPosition(),
    _stepper.targetPosition(),
    _stepper.currentPosition() % NUM_STEPS,
    position,
    _solveMin,
    _solveMax
  );

  if (solved) {
    _logic.serial.print("SOLVED");
  }
}