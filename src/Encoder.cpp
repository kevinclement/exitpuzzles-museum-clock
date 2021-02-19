#include "Arduino.h"
#include "Encoder.h"
#include "logic.h"

#define ENCODER_DEBOUNCE  50

Encoder::Encoder(Logic &logic)
: _logic(logic)
{
}

void Encoder::setup(const char * label, int PIN1, int PIN2) {
  _label = label;
  encoder.attachHalfQuad(PIN1, PIN2);
}

void Encoder::pause() {
  disabled = !disabled;
  if (disabled) {
    encoder.pauseCount();
  } else {
    encoder.resumeCount();
  }
}

void Encoder::clear() {
  Serial.println("clearing internal encoder");
  encoder.clearCount();
  position = 0;

  Serial.print("value now: ");
  Serial.print(encoder.getCount());
  Serial.println();
}

void Encoder::status() {
  _logic.serial.print("%-8s: sensor: %d\r\n", _label, position);
}

void Encoder::handle() {

  if (disabled) {
    return;
  }

  int newPos = encoder.getCount();
  
  if (debug) {
    _logic.serial.print("%-8s: new: %d cur: %d\r\n", _label, newPos, position);
  }

  if (millis() - lastEnc > ENCODER_DEBOUNCE) {
    if (newPos != position) {
      position = newPos;
    }
    lastEnc = millis();
  }
}

