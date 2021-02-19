#pragma once
#include "Arduino.h"
#include <AccelStepper.h>

// VID28-05 is 720 steps per revolution
#define NUM_STEPS 720
#define STEP_SIZE 5

class Logic;

enum motorState {
  IDLE,
  RESETTING,
  RESET,
  FOUND_SENSOR,
  NUDGING,
  NUDGED,
  GAMEON 
};

class VidStepper {
  public:
    VidStepper(Logic &logic, AccelStepper stepper);
    void setup(const char * label, float maxSpeed, float solve);
    void handle();
    
    void move(long relative);
    void nudge();
    void reset();

    long distanceToGo();
    void status();

    motorState state;
    bool solved = false;
    long position = 0;

  private:
    Logic &_logic;
    AccelStepper _stepper;
    void calcPosition();
    float _solveMin = 0;
    float _solveMax = 0;
    const char * _label;

};
