#include "LightSensor.h"
#include <Arduino.h>

LightSensor::LightSensor(int pin, const String &name, const String &description,
                        unsigned long readInterval,
                        float calibrationOffset,
                        float voltageReference,
                        float minLux,
                        float maxLux,
                        float resistance)
    : Sensor(pin, name, description, "light", "lux", readInterval),
      calibrationOffset(calibrationOffset),
      voltageReference(voltageReference),
      minLux(minLux),
      maxLux(maxLux),
      resistance(resistance)
{
}

LightSensor::~LightSensor()
{
}

float LightSensor::readValue()
{
    // Read the analog value
    int rawValue = analogRead(pin);
    
    // Convert to voltage
    float voltage = (rawValue * voltageReference) / 1023.0f;
    
    // Calculate LDR resistance
    float ldrResistance = resistance * (voltageReference - voltage) / voltage;
    
    // Convert resistance to lux using a simplified formula
    // For more accurate readings, you might want to use a lookup table
    // or a more complex conversion formula based on your specific LDR
    float lux = pow(10, (log10(resistance) - log10(ldrResistance)) / 0.7);
    
    // Apply calibration offset and ensure value is within bounds
    lux += calibrationOffset;
    lux = constrain(lux, minLux, maxLux);
    
    return lux;
}

float LightSensor::getCalibrationOffset() const
{
    return calibrationOffset;
}

void LightSensor::setCalibrationOffset(float offset)
{
    calibrationOffset = offset;
}

float LightSensor::getVoltageReference() const
{
    return voltageReference;
}

void LightSensor::setVoltageReference(float voltage)
{
    voltageReference = voltage;
}

float LightSensor::getMinLux() const
{
    return minLux;
}

void LightSensor::setMinLux(float min)
{
    minLux = min;
}

float LightSensor::getMaxLux() const
{
    return maxLux;
}

void LightSensor::setMaxLux(float max)
{
    maxLux = max;
}

float LightSensor::getResistance() const
{
    return resistance;
}

void LightSensor::setResistance(float resistance)
{
    this->resistance = resistance;
}

String LightSensor::getStatus() const
{
    String status = Sensor::getStatus();
    status.remove(status.length() - 1); // Remove closing brace
    
    // Add light-specific status
    status += ",\"calibrationOffset\":" + String(calibrationOffset);
    status += ",\"voltageReference\":" + String(voltageReference);
    status += ",\"minLux\":" + String(minLux);
    status += ",\"maxLux\":" + String(maxLux);
    status += ",\"resistance\":" + String(resistance);
    status += "}";
    
    return status;
} 