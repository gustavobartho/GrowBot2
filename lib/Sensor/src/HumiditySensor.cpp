#include "HumiditySensor.h"
#include <Arduino.h>

HumiditySensor::HumiditySensor(int pin, const String &name, const String &description,
                             unsigned long readInterval,
                             float calibrationOffset,
                             float voltageReference,
                             float minHumidity,
                             float maxHumidity)
    : Sensor(pin, name, description, "humidity", "%", readInterval),
      calibrationOffset(calibrationOffset),
      voltageReference(voltageReference),
      minHumidity(minHumidity),
      maxHumidity(maxHumidity)
{
}

HumiditySensor::~HumiditySensor()
{
}

float HumiditySensor::readValue()
{
    // Read the analog value
    int rawValue = analogRead(pin);
    
    // Convert to voltage
    float voltage = (rawValue * voltageReference) / 1023.0f;
    
    // Convert voltage to humidity percentage
    // This is a simplified linear conversion
    // For more accurate readings, you might want to use a lookup table
    // or a more complex conversion formula based on your specific sensor
    float humidity = (voltage / voltageReference) * (maxHumidity - minHumidity) + minHumidity;
    
    // Apply calibration offset and ensure value is within bounds
    humidity += calibrationOffset;
    humidity = constrain(humidity, minHumidity, maxHumidity);
    
    return humidity;
}

float HumiditySensor::getCalibrationOffset() const
{
    return calibrationOffset;
}

void HumiditySensor::setCalibrationOffset(float offset)
{
    calibrationOffset = offset;
}

float HumiditySensor::getVoltageReference() const
{
    return voltageReference;
}

void HumiditySensor::setVoltageReference(float voltage)
{
    voltageReference = voltage;
}

float HumiditySensor::getMinHumidity() const
{
    return minHumidity;
}

void HumiditySensor::setMinHumidity(float min)
{
    minHumidity = min;
}

float HumiditySensor::getMaxHumidity() const
{
    return maxHumidity;
}

void HumiditySensor::setMaxHumidity(float max)
{
    maxHumidity = max;
}

String HumiditySensor::getStatus() const
{
    String status = Sensor::getStatus();
    status.remove(status.length() - 1); // Remove closing brace
    
    // Add humidity-specific status
    status += ",\"calibrationOffset\":" + String(calibrationOffset);
    status += ",\"voltageReference\":" + String(voltageReference);
    status += ",\"minHumidity\":" + String(minHumidity);
    status += ",\"maxHumidity\":" + String(maxHumidity);
    status += "}";
    
    return status;
} 