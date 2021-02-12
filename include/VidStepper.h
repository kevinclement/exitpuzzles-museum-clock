
#include "Arduino.h"
#include <AccelStepper.h>

// VID28-05 is 720 steps per revolution
#define NUM_STEPS 720
#define STEP_SIZE 5

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
    VidStepper(AccelStepper stepper);
    void setup(float maxSpeed, float solve);
    void handle();
    
    void move(long relative);
    void nudge();
    void reset();

    long distanceToGo();
    long abs();
    void status();

    motorState state;
    bool solved = false;

  private:
    AccelStepper _stepper;
    float _solveMin = 0;
    float _solveMax = 0;

};
