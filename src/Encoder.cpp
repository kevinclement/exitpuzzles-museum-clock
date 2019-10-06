#include "Arduino.h"
#include "Encoder.h"
#include "logic.h"

#define ENCODER_DEBOUNCE  10

#define RESOLUTION        2
#define STEPS             200

int posToTime(int p, int denom) {
  if (p > (STEPS * RESOLUTION)) {
    p %= (STEPS * RESOLUTION);
  } 

  return (int)(round(( p / (float)RESOLUTION / STEPS ) * denom)) % denom;  
}

Encoder::Encoder(Logic &logic)
: _logic(logic)
{  
}

void Encoder::setup(const char * label, int PIN1, int PIN2, int MAX_VALUE) {
  _label = label;
  _MAX_VALUE = MAX_VALUE;
  encoder.attachHalfQuad(PIN1, PIN2);
}

void Encoder::setValue(int pos) {

  // TODO: turn debug on for these eventually
  _logic.serial.print("%-8s: new pos: %d\r\n", _label, pos);

  time = -posToTime(pos, _MAX_VALUE);
  position = pos;

  if (time < 0) {
    time += _MAX_VALUE;
  } else if (time >= _MAX_VALUE) {
    time -= _MAX_VALUE;
  }

}


void Encoder::handle() {

  int newPos = encoder.getCount();
  if (debug) {
    _logic.serial.print("%-8s: pos: %d cur: %d\r\n", _label, newPos, curPos);
  }

  if (newPos != curPos) {
    if (millis() - lastEnc > ENCODER_DEBOUNCE) {
      setValue(newPos);
    }
    lastEnc = millis();
    curPos = newPos;
  }

}

