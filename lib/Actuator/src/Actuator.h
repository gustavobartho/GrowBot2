#ifndef Actuator_h
#define Actuator_h

#include <Arduino.h>
#include <vector>

#include "Device.h"
#include "Behavior.h"

class Actuator : public Device
{
public:
    Behavior *behavior;

public:
    Actuator(unsigned int pinNum, String name, String description, bool normallyClosed);
    ~Actuator();

    void setBehavior(Behavior *behavior);
    void runBehavior();
    void setBehaviorState(BehaviorState state);
};

#endif
