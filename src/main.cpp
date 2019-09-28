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

void setup() {  
  logic.setup();
  logic.serial.print("Museum Grandfather clock by kevinc...\n");
  Serial.println(getFullVersion("museum-clock"));

  logic.serial.registerCommand(SerialCommand("status",  's', &status,     "status",   "gets the status of device"));
  logic.serial.registerCommand(SerialCommand("solve",   'v', &solve,      "solve",   "force a puzzle solve of the device"));
  logic.serial.registerCommand(SerialCommand("reboot",  'r', &reboot,     "reboot",  "software reboot the device"));

  logic.serial.printHelp();
  logic.status();
}

void loop() {
  logic.handle();
}