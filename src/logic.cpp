#include "Arduino.h"
#include "logic.h"
#include "consts.h"

#define HOUR_ENC_1_PIN    4
#define HOUR_ENC_2_PIN    21
#define MINUTE_ENC_1_PIN  25
#define MINUTE_ENC_2_PIN  26

Logic::Logic() 
  : serial(),
    stepmotor(*this),
    hour(*this),
    minute(*this),
    wifi(*this),
    magnet(*this)
{
}

void Logic::setup() {
  serial.setup("ExitClock");
  hour.setup("hour", HOUR_ENC_1_PIN, HOUR_ENC_2_PIN, 12);
  minute.setup("minute", MINUTE_ENC_1_PIN, MINUTE_ENC_2_PIN, 60);
  stepmotor.setup();
  wifi.setup();
  magnet.setup();
}

void Logic::solved() {
  serial.print("Solved!\n");
  status();
}

void Logic::handle() {
  serial.handle();
  magnet.handle();
  wifi.handle();
  hour.handle();
  minute.handle();
  stepmotor.handle();

  if (_hourPos != hour.position) {
    stepmotor.hour_stepper = hour.position / 2;
    _hourPos = hour.position;
  }
}

void Logic::status() {
  char cMsg[254];

  sprintf(cMsg, 
    "status="
      "version:%s,"
      "gitDate:%s,"
      "buildDate:%s,"

      "solved:%s"

      "%s"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,

      "false",

      CRLF);

  serial.print(cMsg);
}
