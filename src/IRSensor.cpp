#include "Arduino.h"
#include "IRSensor.h"
#include "logic.h"

#define IR_BOUNCE 1000

IRSensor::IRSensor(Logic &logic, uint PIN, const char * label)
: _logic(logic), _pin(PIN), _label(label) {
}

void IRSensor::setup() {
  sensor.attach(_pin, INPUT_PULLUP);
  sensor.interval(25);
}

int IRSensor::readSensor() {

  // int val = digitalRead(_pin); 

  // if (debug) {
  //   _logic.serial.print("%-8s: sensor: %d\r\n", _label, val);
  //   delay(100);
  // }

  // if (fakeIt) {
  //   val = 0;
  // }
  
  // return val;
}

void IRSensor::handle() {
  sensor.update();
  
  // if (readSensor() == 1) {
  //   if (solved) {
  //     _logic.serial.print("%s: hall turned off\r\n", _label);
  //   }
  //   _seen_solve_bit_at = 0;
  //   solved = false;
  // } else if (_seen_solve_bit_at == 0) {
  //   _seen_solve_bit_at = millis();
  // } else if (!solved && _seen_solve_bit_at > 0 && millis() - _seen_solve_bit_at > HALL_BOUNCE) {
  //   solved = true;
  //   _logic.serial.print("%s: hall turned on\r\n", _label);
  // }
}