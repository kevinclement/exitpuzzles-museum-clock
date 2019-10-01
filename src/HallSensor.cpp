#include "Arduino.h"
#include "HallSensor.h"
#include "logic.h"

#define HALL_THRESH 10   // TODO: use a proper number
#define HALL_BOUNCE 1000 // TODO: use a proper number

HallSensor::HallSensor(Logic &logic, uint PIN, const char * label)
: _logic(logic), _pin(PIN), _label(label) {
}

void HallSensor::setup() {
}

// fake the sensor so I can code without hardware
void HallSensor::fake() {
  fakeIt = !fakeIt;
}

int HallSensor::readSensor() {
  // TODO: turn this back on when wired
  //int val = analogRead(_pin); 
  int val = 0;

  if (fakeIt) {
    val = HALL_THRESH;
  }
  
  return val;
}

void HallSensor::handle() {
  

  if (readSensor() < HALL_THRESH) {
    if (solved) {
      _logic.serial.print("%s: hall turned off\r\n", _label);
    }
    _seen_solve_bit_at = 0;
    solved = false;
  } else if (_seen_solve_bit_at == 0) {
    _seen_solve_bit_at = millis();
  } else if (!solved && _seen_solve_bit_at > 0 && millis() - _seen_solve_bit_at > HALL_BOUNCE) {
    solved = true;
    _logic.serial.print("%s: hall turned on\r\n", _label);
  }
}

