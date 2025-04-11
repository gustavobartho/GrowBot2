#include "Sensor.h"
#include <Arduino.h>

Sensor::Sensor(int pin, const String &name, const String &description,
               const String &dataType, const String &unit,
               unsigned long readInterval)
    : Device(pin, name, description),
      dataType(dataType),
      unit(unit),
      value(0.0f),
      lastReadTime(0),
      readInterval(readInterval)
{
}

Sensor::~Sensor()
{
}

void Sensor::setup()
{
    Device::setup();
    pinMode(pin, INPUT);
    lastReadTime = millis();
}

void Sensor::update()
{
    Device::update();
    
    if (!isEnabled())
        return;

    unsigned long currentTime = millis();
    if (currentTime - lastReadTime >= readInterval)
    {
        value = readValue();
        lastReadTime = currentTime;
    }
}

float Sensor::readValue()
{
    // Base implementation just reads the analog value
    // Derived classes should override this to implement specific sensor logic
    return analogRead(pin);
}

float Sensor::getValue() const
{
    return value;
}

String Sensor::getDataType() const
{
    return dataType;
}

String Sensor::getUnit() const
{
    return unit;
}

unsigned long Sensor::getReadInterval() const
{
    return readInterval;
}

void Sensor::setReadInterval(unsigned long interval)
{
    readInterval = interval;
}

String Sensor::getStatus() const
{
    String status = Device::getStatus();
    status.remove(status.length() - 1); // Remove closing brace
    
    // Add sensor-specific status
    status += ",\"dataType\":\"" + dataType + "\"";
    status += ",\"unit\":\"" + unit + "\"";
    status += ",\"value\":" + String(value);
    status += ",\"readInterval\":" + String(readInterval);
    status += "}";
    
    return status;
}