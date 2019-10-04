#include "Arduino.h"
#include "HallSensor.h"
#include "logic.h"

#define HALL_THRESH 10   // TODO: use a proper number
#define HALL_BOUNCE 1000 // TODO: use a proper number

HallSensor::HallSensor(Logic &logic, uint PIN, const char * label)
: _logic(logic), _pin(PIN), _label(label) {
}

void HallSensor::setup() {
  pinMode(_pin, INPUT);
}

// fake the sensor so I can code without hardware
void HallSensor::fake() {
  fakeIt = !fakeIt;
}

int HallSensor::readSensor() {
  int val = analogRead(_pin); 

  if (debug) {
    _logic.serial.print("%-8s: sensor: %d\r\n", _label, val);
    delay(100);
  }

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

