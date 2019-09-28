#include "Arduino.h"
#include "Encoder.h"
#include "logic.h"

#define HOUR_ENC_1_PIN    4
#define HOUR_ENC_2_PIN    21
#define MINUTE_ENC_1_PIN  25
#define MINUTE_ENC_2_PIN  26

#define ENCODER_DEBOUNCE  10

#define RESOLUTION        2
#define STEPS             20021

ESP32Encoder hour;
ESP32Encoder minute;

int hourPos                = 0;
int minutePos              = 0;
unsigned long lastHour     = millis();
unsigned long lastMinute   = millis();
unsigned long zeroing      = 0;
unsigned long zeroing_done = 0;

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

void Encoder::setup() {
  hour.attachHalfQuad(HOUR_ENC_1_PIN, HOUR_ENC_2_PIN);
  minute.attachHalfQuad(MINUTE_ENC_1_PIN, MINUTE_ENC_2_PIN);
}

void Encoder::setHour(int hourPos) {
  // hour_stepper = hourPos / RESOLUTION;
  // int posHour = -posToTime(hourPos, 12);

  // HOUR = posHour + STORED_HOUR;
  // if (HOUR < 0) {
  //   HOUR += 12;
  // } else if (HOUR >= 12) {
  //   HOUR -= 12;
  // }
 
  //printStatus();
    
  // TODO: impl

  // EEPROM.put(HOUR_ADDR, HOUR);
  // EEPROM.put(HOUR_STEPPER_ADDR, HOUR_STEPPER);
  // EEPROM.commit();
}

void Encoder::setMinute(int minutePos) {
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
  
  int newHourPos = hour.getCount();
  if (newHourPos != hourPos) {    
    if (millis() - lastHour > ENCODER_DEBOUNCE) {
      setHour(newHourPos);
    }
    lastHour = millis();
    hourPos = newHourPos;
  }

  int newMinutePos =  minute.getCount();
  if (newMinutePos != minutePos) {
    if (millis() - lastMinute > ENCODER_DEBOUNCE) {
      setMinute(newMinutePos);
    }
    lastMinute = millis();
    minutePos = newMinutePos;
  }
}

