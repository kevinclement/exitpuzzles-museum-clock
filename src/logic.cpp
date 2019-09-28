#include "Arduino.h"
#include "logic.h"
#include "consts.h"

Logic::Logic() 
  : serial(),
    stepmotor(*this),
    encoder(*this),
    wifi(*this)
{
}

void Logic::setup() {
  serial.setup("ExitClock");

  encoder.setup();
  stepmotor.setup();
  wifi.setup();
}

void Logic::solved() {
  serial.print("Solved!\n");
  status();
}

void Logic::handle() {
  serial.handle();
  wifi.handle();
  encoder.handle();
  stepmotor.handle();

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
