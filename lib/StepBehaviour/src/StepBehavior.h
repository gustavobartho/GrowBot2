#ifndef StepBehavior_h
#define StepBehavior_h

#include "Behavior.h"
#include <vector>

class StepBehavior : public Behavior
{
private:
    struct Step
    {
        unsigned long duration;
        BehaviorDurationUnit unit;

        Step(unsigned long d, BehaviorDurationUnit u) : duration(d), unit(u) {}
    };

    std::vector<Step> steps;
    unsigned int currentStepIndex;

public:
    StepBehavior();
    void addStep(unsigned long duration, BehaviorDurationUnit unit);
    void run(unsigned int pinNum, bool normallyClosed) override;
    void activate(unsigned int pinNum, bool normallyClosed) override;
    void setState(BehaviorState newState) override;

private:
    void moveToNextStep(unsigned int pinNum, bool normallyClosed);
};

#endif
