#include "Arduino.h"
#include "logic.h"
#include "consts.h"

Logic::Logic() 
  : serial(),
    stepmotor(*this),
    encoder(*this),
    wifi(*this),
    magnet(*this),
    hourSensor(*this, 11),   // TODO: proper pin
    minuteSensor(*this, 12)  // TODO: proper pin
{
}

void Logic::setup() {
  serial.setup("ExitMuseumClock");
  encoder.setup();
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
