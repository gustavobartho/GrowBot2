#ifndef Behavior_h
#define Behavior_h

#include <Arduino.h>
#include <map>

enum class BehaviorState
{
    ON,
    OFF
};

enum class BehaviorType
{
    ON_OFF,
    STEP
};

enum class BehaviorDurationUnit
{
    DAYS,
    HOURS,
    MINUTES,
    SECONDS
};

struct BehaviorStepDuration
{
    int duration;
    BehaviorDurationUnit unit;
};

class Behavior
{
protected:
    unsigned long timeLast;
    double timeSinceLastChange;
    BehaviorDurationUnit timeSinceLastChangeUnit;
    BehaviorState state;
    BehaviorType type;
    bool reversed;
    bool active;

public:
    Behavior() = default;
    Behavior(BehaviorType type, bool reversed);
    virtual ~Behavior() = default;
    BehaviorState getState() const;
    BehaviorType getType() const;
    bool getReversed() const;
    void setReversed(bool reversed, unsigned int pinNum);
    void deactivate(unsigned int pinNum, bool normallyClosed);
    virtual void run(unsigned int pinNum, bool normallyClosed) = 0;
    virtual void activate(unsigned int pinNum, bool normallyClosed) = 0;
    virtual void setState(BehaviorState newState) = 0;

protected:
    int getMillisecondsFromDuration(BehaviorDurationUnit durationUnit);
    unsigned long getTimeDelta(unsigned long timeNow);
    void turnOn(unsigned int pinNum, bool normallyClosed);
    void turnOff(unsigned int pinNum, bool normallyClosed);
    void activateDefault(unsigned int pinNum, bool normallyClosed);
};

#endif
