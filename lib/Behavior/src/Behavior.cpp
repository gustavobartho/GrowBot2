#include "Behavior.h"

Behavior::Behavior(BehaviorType type, bool reversed)
    : timeLast(millis()), timeSinceLastChange(0), state(BehaviorState::OFF),
      type(type), reversed(reversed), active(true)
{
}

BehaviorState Behavior::getState() const
{
    if (reversed && state == BehaviorState::ON)
        return BehaviorState::OFF;

    if (reversed && state == BehaviorState::OFF)
        return BehaviorState::ON;

    return state;
}

BehaviorType Behavior::getType() const
{
    return type;
}

bool Behavior::getReversed() const
{
    return reversed;
}

void Behavior::setReversed(bool reversed, unsigned int pinNum)
{
    if (this->reversed != reversed)
    {
        this->reversed = reversed;
        // Invert the pin state
        digitalWrite(pinNum, !digitalRead(pinNum));
    }
}

unsigned long Behavior::getTimeDelta(unsigned long timeNow)
{
    unsigned long timeDelta = timeNow - timeLast;
    // Handle counter overflow
    if (timeLast > timeNow)
    {
        unsigned long timeMax = std::numeric_limits<unsigned long>::max();
        timeDelta = timeMax - timeLast + timeNow;
    }
    return timeDelta;
}

void Behavior::activateDefault(unsigned int pinNum, bool normallyClosed)
{
    timeLast = millis();
    active = true;
    state = BehaviorState::OFF;
    timeSinceLastChange = 0;
    digitalWrite(pinNum, normallyClosed ? HIGH : LOW);
}

void Behavior::deactivate(unsigned int pinNum, bool normallyClosed)
{
    timeLast = millis();
    active = false;
    state = BehaviorState::OFF;
    timeSinceLastChange = 0;
    digitalWrite(pinNum, normallyClosed ? HIGH : LOW);
}

void Behavior::turnOn(unsigned int pinNum, bool normallyClosed)
{
    // Invert the output if normallyClosed or reversed
    digitalWrite(pinNum, (normallyClosed != reversed) ? LOW : HIGH);
}

void Behavior::turnOff(unsigned int pinNum, bool normallyClosed)
{
    digitalWrite(pinNum, (normallyClosed != reversed) ? HIGH : LOW);
}

int Behavior::getMillisecondsFromDuration(BehaviorDurationUnit durationUnit)
{
    static const std::map<BehaviorDurationUnit, int> durationMap{
        {BehaviorDurationUnit::SECONDS, 1000},
        {BehaviorDurationUnit::MINUTES, 60000},
        {BehaviorDurationUnit::HOURS, 3600000},
        {BehaviorDurationUnit::DAYS, 86400000}};

    auto it = durationMap.find(durationUnit);
    if (it != durationMap.end())
    {
        return it->second;
    }
    // Handle invalid durationUnit, return a default value, or throw an exception.
    return 0;
}
