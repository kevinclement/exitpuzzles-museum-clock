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
}

void disableEncoder(int) {
  logic.serial.print("toggling encoder disabling of readings...%s", CRLF);
  logic.hour.pause();
  logic.minute.pause();
}

void hourIncrement(int) {
  logic.serial.print("incrementing hour...%s", CRLF);
  
  int cur = logic.hour.encoder.getCount();
  logic.hour.encoder.setCount(cur - 10);
}

void hourDec(int) {
  logic.serial.print("decrementing hour...%s", CRLF);
  
  int cur = logic.hour.encoder.getCount();
  logic.hour.encoder.setCount(cur + 10);
}

void resetHands(int) {
  logic.serial.print("resetting hands...%s", CRLF);
  logic.resetHands();
}

void minuteIncrement(int) {
  logic.serial.print("incrementing minute...%s", CRLF);
  
  int cur = logic.minute.encoder.getCount();
  logic.minute.encoder.setCount(cur - 10);
}

void minuteDec(int) {
  logic.serial.print("decrementing minute...%s", CRLF);
  
  int cur = logic.minute.encoder.getCount();
  logic.minute.encoder.setCount(cur + 10);
}

void motorToggle(int) {
  logic.serial.print("toggling stepper motor...%s", CRLF);
  // TODO: fix
  // logic.stepmotor._enabled = !logic.stepmotor._enabled;
  
  logic.status();
}

void audio(int) {
  logic.serial.print("playing solve audio...%s", CRLF);
  logic.audio.playTone();
}

void setup() {
  logic.setup();
  logic.serial.print("Museum Grandfather clock by kevinc...\n");
  Serial.println(getFullVersion("museum-clock"));

  logic.serial.registerCommand(SerialCommand("status",         's', &status,          "status",         "gets the status of device"));
  logic.serial.registerCommand(SerialCommand("solve",          'v', &solve,           "solve",          "force a puzzle solve of the device"));
  logic.serial.registerCommand(SerialCommand("debug",          'x', &debug,           "debug",          "toggle debugging of encoders"));
  logic.serial.registerCommand(SerialCommand("disableEncoder", 'y', &disableEncoder,  "disableEncoder", "disableEncoder reading"));
  logic.serial.registerCommand(SerialCommand("hour",           'h', &hourIncrement,   "hour",           "increment hour count as an override"));
  logic.serial.registerCommand(SerialCommand("hourDec",        'o', &hourDec,         "hourDec",        "decrement hour count as an override"));
  logic.serial.registerCommand(SerialCommand("reset",          'a', &resetHands,      "reset",          "reset the hands to the starting positions"));
  logic.serial.registerCommand(SerialCommand("minute",         'm', &minuteIncrement, "minute",         "increment minute count as an override"));
  logic.serial.registerCommand(SerialCommand("minDec",         'i', &minuteDec,       "minDec",         "decrement minute count as an override"));
  logic.serial.registerCommand(SerialCommand("motor",          't', &motorToggle,     "motor",          "toggle the motor enabled"));
  logic.serial.registerCommand(SerialCommand("reboot",         'r', &reboot,          "reboot",         "software reboot the device"));
  logic.serial.registerCommand(SerialCommand("audio",          'z', &audio,           "audio",          "play the solve audio"));

  logic.serial.printHelp();
  logic.status();
}

void loop() {
  logic.handle();
}