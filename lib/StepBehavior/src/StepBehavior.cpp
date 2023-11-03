#include "StepBehavior.h"

StepBehavior::StepBehavior() : Behavior(BehaviorType::STEP, false), currentStepIndex(0) {}

// ---------------------------------------------

void StepBehavior::setState(BehaviorState newState) {}

// ---------------------------------------------

void StepBehavior::addStep(unsigned long duration, BehaviorDurationUnit unit)
{
    steps.emplace_back(duration, unit);
}

// ---------------------------------------------

void StepBehavior::run(unsigned int pinNum, bool normallyClosed)
{
    updateTimeSinceLastChange(millis());

    if (checkFinishedStep())
    {
        moveToNextStep(pinNum, normallyClosed);
        resetTimeSinceLastChange();
    }
}

// ---------------------------------------------

void StepBehavior::activate(unsigned int pinNum, bool normallyClosed)
{
    currentStepIndex = 0;

    if (steps.empty())
    {
        this->turnOff(pinNum, normallyClosed);
        return;
    }

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

// ---------------------------------------------

bool StepBehavior::checkFinishedStep()
{
    Step &currentStep = steps[currentStepIndex];
    switch (currentStep.unit)
    {
    case BehaviorDurationUnit::SECONDS:
        return timeSinceLastChange[3] >= currentStep.duration;
    case BehaviorDurationUnit::MINUTES:
        return timeSinceLastChange[2] >= currentStep.duration;
    case BehaviorDurationUnit::HOURS:
        return timeSinceLastChange[1] >= currentStep.duration;
    case BehaviorDurationUnit::DAYS:
        return timeSinceLastChange[0] >= currentStep.duration;
    default:
        return false;
    }
}

// ---------------------------------------------

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

// ---------------------------------------------

BehaviorStatusData StepBehavior::getStatusData()
{
    BehaviorStatusData data = {};
    setStatusBaseData(&data);
    data.step = currentStepIndex;
    return data;
}