#include "InputBoundBehavior.h"

InputBoundBehavior::InputBoundBehavior(String sensorName, float threshold, ComparisonType comparisonType, unsigned long checkInterval)
    : Behavior(BehaviorType::INPUT_BOUND, false),
      sensorName(sensorName),
      threshold(threshold),
      comparisonType(comparisonType),
      checkInterval(checkInterval),
      lastCheckTime(0)
{
}

void InputBoundBehavior::activate(unsigned int pinNum, bool normallyClosed)
{
    // Initialize the behavior
    this->activateDefault(pinNum, normallyClosed);
    this->lastCheckTime = millis();
}

void InputBoundBehavior::run(unsigned int pinNum, bool normallyClosed)
{
    unsigned long currentTime = millis();

    // Check if enough time has passed
    if (currentTime - lastCheckTime >= checkInterval)
    {
        // Get the sensor by name
        Sensor *sensor = dynamic_cast<Sensor *>(DeviceManager::getInstance().getDevice(sensorName));

        if (sensor != nullptr)
        {
            // Check the sensor value against the threshold
            float sensorValue = sensor->getValue();
            bool conditionMet = evaluateCondition(sensorValue);

            // Update state based on condition
            BehaviorState newState = conditionMet ? BehaviorState::ON : BehaviorState::OFF;

            // Only change pin state if the state actually changed
            if (newState != state)
            {
                state = newState;

                if (state == BehaviorState::ON)
                {
                    turnOn(pinNum, normallyClosed);
                }
                else
                {
                    turnOff(pinNum, normallyClosed);
                }
            }
        }

        lastCheckTime = currentTime;
    }
}

void InputBoundBehavior::setState(BehaviorState newState)
{
    // For input-bound behaviors, the state is determined by the sensor reading
    // This function is effectively a no-op
}

void InputBoundBehavior::setThreshold(float newThreshold)
{
    threshold = newThreshold;
}

void InputBoundBehavior::setComparisonType(ComparisonType type)
{
    comparisonType = type;
}

void InputBoundBehavior::setCheckInterval(unsigned long interval)
{
    checkInterval = interval;
}

bool InputBoundBehavior::evaluateCondition(float sensorValue)
{
    switch (comparisonType)
    {
    case ComparisonType::GREATER_THAN:
        return sensorValue > threshold;

    case ComparisonType::LESS_THAN:
        return sensorValue < threshold;

    case ComparisonType::EQUAL_TO:
        return abs(sensorValue - threshold) < 0.001; // Approximate comparison for floats

    default:
        return false;
    }
}