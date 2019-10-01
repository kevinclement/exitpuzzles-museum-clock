#include "Arduino.h"
#include "HallSensor.h"
#include "logic.h"

HallSensor::HallSensor(Logic &logic, uint PIN)
: _logic(logic), _pin(PIN)
{
    
}

void HallSensor::setup() {

}

void HallSensor::handle() {
  
}

