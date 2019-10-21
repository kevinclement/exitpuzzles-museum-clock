#include <Arduino.h>
#include "logic.h"
#include "consts.h"
#include "version.h"

// Globals
Logic logic;

void reboot(int) {
  ESP.restart();
}

void status(int) {
  logic.status();
}

void solve(int) {
  logic.serial.print("forcing a puzzle solve...%s", CRLF);
  logic.solved();
}

void debug(int) {
  logic.serial.print("toggling encoder debugging...%s", CRLF);
  logic.hour.debug = !logic.hour.debug;
  logic.minute.debug = !logic.minute.debug;
  //logic.hourSensor.debug = !logic.hourSensor.debug;
}

void hourIncrement(int) {
  logic.serial.print("incrementing hour...%s", CRLF);
  
  int cur = logic.hour.encoder.getCount();
  logic.hour.encoder.setCount(cur - 10);
}

void minuteIncrement(int) {
  logic.serial.print("incrementing minute...%s", CRLF);
  
  int cur = logic.minute.encoder.getCount();
  logic.minute.encoder.setCount(cur - 10);
}

void motorToggle(int) {
  logic.serial.print("toggling stepper motor...%s", CRLF);
  logic.stepmotor._enabled = !logic.stepmotor._enabled;
  logic.status();

}

void setup() {  
  logic.setup();
  logic.serial.print("Museum Grandfather clock by kevinc...\n");
  Serial.println(getFullVersion("museum-clock"));

  logic.serial.registerCommand(SerialCommand("status",  's', &status,          "status",  "gets the status of device"));
  logic.serial.registerCommand(SerialCommand("solve",   'v', &solve,           "solve",   "force a puzzle solve of the device"));
  logic.serial.registerCommand(SerialCommand("debug",   'x', &debug,           "debug",   "toggle debugging of encoders"));
  logic.serial.registerCommand(SerialCommand("hour",    'h', &hourIncrement,   "hour",    "increment hour count as an override"));
  logic.serial.registerCommand(SerialCommand("minute",  'm', &minuteIncrement, "minute",  "increment minute count as an override"));
  logic.serial.registerCommand(SerialCommand("motor",   't', &motorToggle,     "motor",   "toggle the motor enabled"));
  logic.serial.registerCommand(SerialCommand("reboot",  'r', &reboot,          "reboot",  "software reboot the device"));

  logic.serial.printHelp();
  logic.status();
}

void loop() {
  logic.handle();
}