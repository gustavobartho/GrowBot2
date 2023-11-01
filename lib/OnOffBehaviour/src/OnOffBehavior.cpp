#include "OnOffBehavior.h"

OnOffBehavior::OnOffBehavior() : Behavior(BehaviorType::ON_OFF, false)
{
    state = BehaviorState::OFF;
    previousState = state;
}

// ---------------------------------------------

void OnOffBehavior::activate(unsigned int pinNum, bool normallyClosed)
{
    activateDefault(pinNum, normallyClosed);
}

// ---------------------------------------------

void OnOffBehavior::run(unsigned int pinNum, bool normallyClosed)
{
    updateTimeSinceLastChange(millis());

    if (previousState != state)
    {
        if (state == BehaviorState::ON)
            turnOn(pinNum, normallyClosed);

        else if (state == BehaviorState::OFF)
            turnOff(pinNum, normallyClosed);

        resetTimeSinceLastChange();
    }
    previousState = state;
}

// ---------------------------------------------

void OnOffBehavior::setState(BehaviorState newState)
{
    state = newState;
}

// ---------------------------------------------

BehaviorStatusData OnOffBehavior::getStatusData()
{
    BehaviorStatusData data = {};
    setStatusBaseData(&data);
    return data;
}

// ---------------------------------------------
