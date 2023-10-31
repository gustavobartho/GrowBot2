#include "Behavior.h"

Behavior::Behavior(BehaviorType type, bool reversed)
    : timeLast(millis()),
      timeSinceLastChange({0, 0, 0, 0}),
      state(BehaviorState::OFF),
      type(type),
      reversed(reversed),
      active(true) {}

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

void Behavior::updateTimeSinceLastChange(unsigned long timeNow)
{
    unsigned long timeDelta = timeNow - timeLast;
    // Handle counter overflow
    if (timeLast > timeNow)
    {
        unsigned long timeMax = std::numeric_limits<unsigned long>::max();
        timeDelta = timeMax - timeLast + timeNow;
    }

    if (timeDelta >= 1000)
    {
        timeSinceLastChange[3] += 1;
        timeLast = timeNow - (timeDelta - 1000);
    }

    if (timeSinceLastChange[3] >= 60)
    {
        timeSinceLastChange[3] = 0;
        timeSinceLastChange[2] += 1;
    }

    if (timeSinceLastChange[2] >= 60)
    {
        timeSinceLastChange[2] = 0;
        timeSinceLastChange[1] += 1;
    }

    if (timeSinceLastChange[1] >= 24)
    {
        timeSinceLastChange[1] = 0;
        timeSinceLastChange[0] += 1;
    }
}

void Behavior::resetTimeSinceLastChange()
{
    timeLast = millis();
    for (int i = 0; i < sizeof(timeSinceLastChange); i++)
        timeSinceLastChange[i] = 0;
}

void Behavior::activateDefault(unsigned int pinNum, bool normallyClosed)
{
    timeLast = millis();
    active = true;
    state = reversed ? BehaviorState::ON : BehaviorState::OFF;
    resetTimeSinceLastChange();
    digitalWrite(pinNum, reversed != normallyClosed ? HIGH : LOW);
}

void Behavior::deactivate(unsigned int pinNum, bool normallyClosed)
{
    timeLast = millis();
    active = false;
    state = BehaviorState::OFF;
    resetTimeSinceLastChange();
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

String Behavior::getTypeFormatted()
{
    switch (this->type)
    {
    case BehaviorType::ON_OFF:
        return String("ON-OFF");
    case BehaviorType::STEP:
        return String('STEP');
    default:
        return String("UNKNOWN");
    }
}

String Behavior::getStateFormatted()
{
    switch (this->state)
    {
    case BehaviorState::ON:
        return String("ON");
    case BehaviorState::OFF:
        return String("OFF");
    default:
        return String("UNKNOWN");
    }
}

String Behavior::getTimeSinceLastChangeFormatted()
{
    String message = "";

    if (timeSinceLastChange[0] > 0)
        message += String(timeSinceLastChange[0]) + "days ";

    if (timeSinceLastChange[1] > 0)
        message += String(timeSinceLastChange[0]) + "hours ";

    if (timeSinceLastChange[2] > 0)
        message += String(timeSinceLastChange[0]) + "minutes ";

    message += String(timeSinceLastChange[0]) + "seconds";

    return message;
}
