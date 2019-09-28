#include "Arduino.h"
#include "Encoder.h"
#include "logic.h"

#define HOUR_ENC_1_PIN    4
#define HOUR_ENC_2_PIN    21
#define MINUTE_ENC_1_PIN  25
#define MINUTE_ENC_2_PIN  26

#define ENCODER_DEBOUNCE  10

ESP32Encoder hour;
ESP32Encoder minute;

int hourPos                = 0;
int minutePos              = 0;
unsigned long lastHour     = millis();
unsigned long lastMinute   = millis();
unsigned long zeroing      = 0;
unsigned long zeroing_done = 0;

Encoder::Encoder(Logic &logic)
: _logic(logic)
{  
}

void Encoder::setup() {
  hour.attachHalfQuad(HOUR_ENC_1_PIN, HOUR_ENC_2_PIN);
  minute.attachHalfQuad(MINUTE_ENC_1_PIN, MINUTE_ENC_2_PIN);
}

void Encoder::handle() {
}
