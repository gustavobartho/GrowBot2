#ifndef OnOffBehavior_h
#define OnOffBehavior_h

#include "Behavior.h"

class OnOffBehavior : public Behavior
{
private:
    BehaviorState previousState;

public:
    OnOffBehavior();
    void run(unsigned int pinNum, bool normallyClosed) override;
    void activate(unsigned int pinNum, bool normallyClosed) override;
    void setState(BehaviorState newState) override;
    BehaviorStatusData getStatusData() override;
};

#endif
