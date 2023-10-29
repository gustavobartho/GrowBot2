#include "StepBehavior.h"

StepBehavior::StepBehavior() : Behavior(BehaviorType::STEP, false), currentStepIndex(0) {}

void StepBehavior::setState(BehaviorState newState) {}

void StepBehavior::addStep(unsigned long duration, BehaviorDurationUnit unit)
{
    steps.emplace_back(duration, unit);
}

void StepBehavior::run(unsigned int pinNum, bool normallyClosed)
{
    Step &currentStep = steps[currentStepIndex];
    unsigned long timeNow = millis();
    unsigned long elapsedTime = getTimeDelta(timeNow);
    unsigned long stepDurationInMillis = currentStep.duration * getMillisecondsFromDuration(currentStep.unit);

    if (elapsedTime >= stepDurationInMillis)
    {
        moveToNextStep(pinNum, normallyClosed);
        timeLast = timeNow - (elapsedTime - stepDurationInMillis);
    }
}

void StepBehavior::activate(unsigned int pinNum, bool normallyClosed)
{
    // Start with the first step
    currentStepIndex = 0;
    // Activate the initial step's state
    if (steps.empty())
    {
        // If there are no steps, default to OFF
        this->turnOff(pinNum, normallyClosed);
    }
    else
    {
        Step &currentStep = steps[currentStepIndex];
        if (state == BehaviorState::ON)
        {
            this->turnOn(pinNum, normallyClosed);
        }
        else
        {
            this->turnOff(pinNum, normallyClosed);
        }
    }
}

void StepBehavior::moveToNextStep(unsigned int pinNum, bool normallyClosed)
{
    if (steps.empty())
        return;

    currentStepIndex = (currentStepIndex + 1) % steps.size();

    state = BehaviorState::ON;
    if (currentStepIndex % 2 == 0)
        state = BehaviorState::OFF;

    if (state == BehaviorState::ON)
    {
        this->turnOn(pinNum, normallyClosed);
    }
    else
    {
        this->turnOff(pinNum, normallyClosed);
    }
}