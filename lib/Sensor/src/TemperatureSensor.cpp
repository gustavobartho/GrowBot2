#include "TemperatureSensor.h"
#include <Arduino.h>

TemperatureSensor::TemperatureSensor(int pin, const String &name, const String &description,
                                   unsigned long readInterval,
                                   float calibrationOffset,
                                   float voltageReference,
                                   float resistance,
                                   float betaCoefficient)
    : Sensor(pin, name, description, "temperature", "°C", readInterval),
      calibrationOffset(calibrationOffset),
      voltageReference(voltageReference),
      resistance(resistance),
      betaCoefficient(betaCoefficient)
{
}

TemperatureSensor::~TemperatureSensor()
{
}

float TemperatureSensor::readValue()
{
    // Read the analog value
    int rawValue = analogRead(pin);
    
    // Convert to voltage
    float voltage = (rawValue * voltageReference) / 1023.0f;
    
    // Calculate thermistor resistance
    float thermistorResistance = resistance * (voltageReference - voltage) / voltage;
    
    // Calculate temperature using Steinhart-Hart equation
    float steinhart = log(thermistorResistance / resistance) / betaCoefficient;
    steinhart += 1.0f / (25.0f + 273.15f);  // 25°C in Kelvin
    steinhart = 1.0f / steinhart;
    steinhart -= 273.15f;  // Convert to Celsius
    
    // Apply calibration offset
    return steinhart + calibrationOffset;
}

float TemperatureSensor::getCalibrationOffset() const
{
    return calibrationOffset;
}

void TemperatureSensor::setCalibrationOffset(float offset)
{
    calibrationOffset = offset;
}

float TemperatureSensor::getVoltageReference() const
{
    return voltageReference;
}

void TemperatureSensor::setVoltageReference(float voltage)
{
    voltageReference = voltage;
}

float TemperatureSensor::getResistance() const
{
    return resistance;
}

void TemperatureSensor::setResistance(float resistance)
{
    this->resistance = resistance;
}

float TemperatureSensor::getBetaCoefficient() const
{
    return betaCoefficient;
}

void TemperatureSensor::setBetaCoefficient(float beta)
{
    betaCoefficient = beta;
}

String TemperatureSensor::getStatus() const
{
    String status = Sensor::getStatus();
    status.remove(status.length() - 1); // Remove closing brace
    
    // Add temperature-specific status
    status += ",\"calibrationOffset\":" + String(calibrationOffset);
    status += ",\"voltageReference\":" + String(voltageReference);
    status += ",\"resistance\":" + String(resistance);
    status += ",\"betaCoefficient\":" + String(betaCoefficient);
    status += "}";
    
    return status;
} 