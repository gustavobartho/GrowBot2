#ifndef InputBoundBehavior_h
#define InputBoundBehavior_h

#include "Behavior.h"
#include "Sensor.h"
#include "DeviceManager.h"

enum class ComparisonType
{
    GREATER_THAN,
    LESS_THAN,
    EQUAL_TO
};

class InputBoundBehavior : public Behavior
{
private:
    String sensorName;
    float threshold;
    ComparisonType comparisonType;
    unsigned long checkInterval; // ms between sensor checks
    unsigned long lastCheckTime;

public:
    InputBoundBehavior(String sensorName, float threshold, ComparisonType comparisonType = ComparisonType::GREATER_THAN, unsigned long checkInterval = 1000);

    void run(unsigned int pinNum, bool normallyClosed) override;
    void activate(unsigned int pinNum, bool normallyClosed) override;
    void setState(BehaviorState newState) override;

    void setThreshold(float newThreshold);
    void setComparisonType(ComparisonType type);
    void setCheckInterval(unsigned long interval);

private:
    bool evaluateCondition(float sensorValue);
};

#endif