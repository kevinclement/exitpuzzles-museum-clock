#include "Arduino.h"
#include "logic.h"
#include "consts.h"

Logic::Logic() 
  : serial(),
    wifi(*this),
    magnet(*this),
    hourSensor(*this, 11, "HOUR"),     // TODO: proper pin
    minuteSensor(*this, 12, "MINUTE")  // TODO: proper pin
{
}

void Logic::setup() {
  serial.setup("ExitMuseumClock");
  minuteSensor.setup();
  hourSensor.setup();

  wifi.setup();
  magnet.setup();
}

void Logic::solved() {
  serial.print("Solved!\n");
  _solved = true;
  status();
}

void Logic::handle() {
  serial.handle();
  wifi.handle();
  minuteSensor.handle();
  hourSensor.handle();
  magnet.handle();

  if (hourSensor.solved != _hs || minuteSensor.solved != _ms) {
    _hs = hourSensor.solved;
    _ms = minuteSensor.solved;

    if (!_solved && _hs && _ms) {
      solved();
    } else {
      status();
    }
  }
}

void Logic::status() {
  char cMsg[254];

  sprintf(cMsg, 
    "status="
      "version:%s,"
      "gitDate:%s,"
      "buildDate:%s,"

      "hs:%s,"
      "ms:%s,"
      "solved:%s"

      "%s"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,

      _hs ? "true" : "false",
      _ms ? "true" : "false",
      _solved ? "true" : "false",

      CRLF);

  serial.print(cMsg);
}
