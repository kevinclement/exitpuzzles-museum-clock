#include "Arduino.h"
#include "Stepper.h"
#include "logic.h"

#define LOCK_PIN 12

Magnet::Magnet(Logic &logic)
: _logic(logic)
{  
}

void Magnet::setup() {
  pinMode(LOCK_PIN, OUTPUT);
}

void Magnet::handle() {
  digitalWrite(LOCK_PIN, _enabled ? HIGH : LOW);
}
