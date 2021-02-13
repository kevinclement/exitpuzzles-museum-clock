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

void Encoder::setValue(int pos) {
  position = pos;
}

void Encoder::status() {
  Serial.print(_label);
  Serial.print(" cur: ");
  Serial.print(curPos);
  Serial.println();
}

void Encoder::handle() {

  if (disabled) {
    return;
  }

  int newPos = encoder.getCount();
  
  if (debug) {
    Serial.print(_label);
    Serial.print(": pos: ");
    Serial.print(newPos);
    Serial.print(" cur: ");
    Serial.print(curPos);
    Serial.println();
  }

  if (millis() - lastEnc > ENCODER_DEBOUNCE) {
    if (newPos != curPos) {
      setValue(newPos);
      curPos = newPos;
    }
    lastEnc = millis();
  }

}

