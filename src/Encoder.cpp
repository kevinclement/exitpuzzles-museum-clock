#include "Arduino.h"
#include "Encoder.h"
#include "logic.h"

#define ENCODER_DEBOUNCE  10

#define RESOLUTION        2
#define STEPS             20021

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

void Encoder::setup(const char * label, int PIN1, int PIN2) {
  _label = label;
  encoder.attachHalfQuad(PIN1, PIN2);
}

void Encoder::setValue(int pos) {
  
  Serial.printf("%-8s: new pos: %d\r\n", _label, pos);
  // minute_stepper = minutePos / RESOLUTION;
  // int posMinute = -posToTime(minutePos, 60);
  
  // MINUTE = posMinute + STORED_MINUTE;
  // if (MINUTE < 0) {
  //   MINUTE += 60;
  // } else if (MINUTE >= 60) {
  //   MINUTE -= 60;
  // }

  // printStatus();
    
  // EEPROM.put(MINUTE_ADDR, MINUTE);
  // EEPROM.put(MINUTE_STEPPER_ADDR, MINUTE_STEPPER);
  // EEPROM.commit();
}


void Encoder::handle() {

  int newPos = encoder.getCount();
  if (debug) {
    Serial.printf("%-8s: pos: %d cur: %d\r\n", _label, newPos, curPos);
  }

  if (newPos != curPos) {
    if (millis() - lastEnc > ENCODER_DEBOUNCE) {
      setValue(newPos);
    }
    lastEnc = millis();
    curPos = newPos;
  }
  
}

