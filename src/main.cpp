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
  logic.hour.encoder.setCount(cur + 10);
}

void minuteIncrement(int) {
  logic.serial.print("incrementing minute...%s", CRLF);
  
  int cur = logic.minute.encoder.getCount();
  logic.minute.encoder.setCount(cur + 10);
}

void motorToggle(int) {
  logic.serial.print("toggling stepper motor...%s", CRLF);
  logic.stepmotor._enabled = !logic.stepmotor._enabled;
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

  // if (command == "enable") {
  //   p("enabling motors...%s", CRLF);
  //   ENABLED = true;

  // else if (command == "zero") {
  //   // if we always started at 0,0, then all
  //   // we'd need to do is set stepper back to 0,0 and we'd be done
  //   // however, given that we can stop the device anywhere, we need to offset by that ammount
  //   int hDest = -200 - HOUR_STEPPER_STORED;
  //   int mDest = -200 - MINUTE_STEPPER_STORED;
  //   p("resetting to zero...%s", CRLF);
  //   p("  hour step: %d stored: %d dest: %d%s", HOUR_STEPPER, HOUR_STEPPER_STORED, hDest, CRLF);
  //   p("  minute step: %d stored: %d dest: %d%s", MINUTE_STEPPER, MINUTE_STEPPER_STORED, mDest, CRLF);
    
  //   zeroing = millis();
  //   HOUR_STEPPER = hDest;
  //   MINUTE_STEPPER = mDest;

  // else if (command == "disable") {
  //   p("disabling motors...%s", CRLF);
  //   ENABLED = false;

  // else if (command == "unlock") {
  //   p("unlocking door...%s", CRLF);
  //   LOCKED_OVERRIDE = true;

  // else if (command == "lock") {
  //   p("locking door...%s", CRLF);
  //   LOCKED_OVERRIDE = false;

  // else if (command == "solved") {
  //   p("solved: h:%d m:%d => ", HOUR, MINUTE);
  //   p(solved() + CRLF);


  // else if (command == "state") {
  //   printStatus();

  // else if (command == "t") {
  //    p("  hour: %d minute: %d%s", TARGET_HOUR, TARGET_MINUTE, CRLF);

  // else if (command == "debounce") {
  //   p("setting debounce to '%d'...%s", value, CRLF);
  //   TARGET_DEBOUNCE = value;
  //   EEPROM.put(TARGET_DEBOUNCE_ADDR, TARGET_DEBOUNCE);
  //   EEPROM.commit();

  // else if (command.startsWith("target")) {
  //   String timeStr = command.substring(7,command.length());
  //   int colonLoc = timeStr.indexOf(':');
  //   TARGET_HOUR = timeStr.substring(0,colonLoc).toInt();
  //   TARGET_MINUTE = timeStr.substring(colonLoc+1, timeStr.length()).toInt();

  //   p("  hour: %d minute: %d%s", TARGET_HOUR, TARGET_MINUTE, CRLF);

  //   EEPROM.put(TARGET_HOUR_ADDR, TARGET_HOUR);
  //   EEPROM.put(TARGET_MINUTE_ADDR, TARGET_MINUTE);
  //   EEPROM.commit();

  // else if (command == "reset") {
  //   p("resetting...%s", CRLF);
  //   resetState();

  // else if (command == "m") {
  //   p("setting minute pos to '%d'...%s", value, CRLF);
  //   MINUTE_STEPPER = value;
  //   EEPROM.put(MINUTE_STEPPER_ADDR, value);
  //   EEPROM.commit();
  
  // else if (command == "ms") {
  //   p("setting stored minute to '%d'...%s", value, CRLF);
  //   STORED_MINUTE = value;
  //   EEPROM.put(MINUTE_ADDR, value);
  //   EEPROM.commit();
  
  // else if (command == "h") {
  //   p("setting hour pos to '%d'...%s", value, CRLF);
  //   HOUR_STEPPER = value;
  //   EEPROM.put(HOUR_STEPPER_ADDR, value);
  //   EEPROM.commit();
  
  
  // else if (command == "hs") {
  //   p("setting stored hour to '%d'...%s", value, CRLF);
  //   STORED_HOUR = value;
  //   EEPROM.put(HOUR_ADDR, value);
  //   EEPROM.commit();
  