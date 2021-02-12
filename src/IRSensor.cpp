#include "Arduino.h"
#include "IRSensor.h"
#include "logic.h"

#define IR_BOUNCE 25

IRSensor::IRSensor(Logic &logic, uint PIN, const char * label)
: _logic(logic), _pin(PIN), _label(label) {
}

void IRSensor::setup() {
  sensor.attach(_pin, INPUT_PULLUP);
  sensor.interval(IR_BOUNCE);
}

void IRSensor::handle() {
  sensor.update();
}