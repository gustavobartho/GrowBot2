#include "OnOffBehavior.h"

OnOffBehavior::OnOffBehavior() : Behavior(BehaviorType::ON_OFF, false)
{
    this->state = BehaviorState::OFF;
    this->previousState = this->state;
}

void OnOffBehavior::activate(unsigned int pinNum, bool normallyClosed)
{
    this->activateDefault(pinNum, normallyClosed);
}

void OnOffBehavior::run(unsigned int pinNum, bool normallyClosed)
{
    if (this->previousState != this->state)
    {
        if (this->state == BehaviorState::ON)
            this->turnOn(pinNum, normallyClosed);

        else if (this->state == BehaviorState::OFF)
            this->turnOff(pinNum, normallyClosed);
    }
    this->previousState = this->state;
}

void OnOffBehavior::setState(BehaviorState newState)
{
    this->state = newState;
}
