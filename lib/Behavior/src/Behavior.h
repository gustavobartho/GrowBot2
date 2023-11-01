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

struct BehaviorStatusData
{
    BehaviorState state;
    BehaviorType type;
    bool reversed;
    unsigned int step = 0;

    String typeFormatted;
    String stateFormatted;
    String timeSinceLastChangeFormatted;
};

class Behavior
{
protected:
    unsigned long timeLast;
    double timeSinceLastChange[4];
    BehaviorState state;
    BehaviorType type;
    bool reversed;
    bool active;

public:
    Behavior() = default;
    Behavior(BehaviorType type, bool reversed);
    ~Behavior() = default;

    virtual void activate(unsigned int pinNum, bool normallyClosed) = 0;
    virtual void run(unsigned int pinNum, bool normallyClosed) = 0;
    virtual void setState(BehaviorState newState) = 0;
    virtual BehaviorStatusData getStatusData() = 0;

    BehaviorState getState() const;
    BehaviorType getType() const;
    bool getReversed() const;
    String getTypeFormatted() const;
    String getStateFormatted() const;
    String getTimeSinceLastChangeFormatted() const;

protected:
    void turnOn(unsigned int pinNum, bool normallyClosed);
    void turnOff(unsigned int pinNum, bool normallyClosed);
    void activateDefault(unsigned int pinNum, bool normallyClosed);
    void setReversed(bool reversed, unsigned int pinNum);
    void deactivate(unsigned int pinNum, bool normallyClosed);
    void updateTimeSinceLastChange(unsigned long timeNow);
    void resetTimeSinceLastChange();
    void setStatusBaseData(BehaviorStatusData *data);
};

#endif
