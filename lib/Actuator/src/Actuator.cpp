#include "Actuator.h"

Actuator::Actuator(unsigned int pinNum, String name, String description, bool normallyClosed)
    : Device(pinNum, OUTPUT, name, description, normallyClosed), behavior(nullptr) {}

Actuator::~Actuator()
{
    if (behavior)
    {
        delete behavior;
    }
}

void Actuator::setBehavior(Behavior *newBehavior)
{
    if (behavior)
    {
        delete behavior; // Clean up the previous behavior.
    }
    behavior = newBehavior;
    behavior->activate(getPin(), normallyClosed);
    runBehavior();
}

void Actuator::runBehavior()
{
    if (behavior)
    {
        behavior->run(getPin(), normallyClosed);
    }
}

void Actuator::setBehaviorState(BehaviorState state)
{
    if (behavior)
    {
        behavior->setState(state);
    }
}