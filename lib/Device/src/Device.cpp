#include "Device.h"

Device::Device(unsigned int pinNum, uint8_t mode, const String &name, const String &description, bool normallyClosed)
    : pinNum(pinNum), mode(mode), name(name), description(description), normallyClosed(normallyClosed)
{
    pinMode(this->pinNum, OUTPUT);
}

void Device::setPin(unsigned int pinNum)
{
    this->pinNum = pinNum;
    pinMode(this->pinNum, OUTPUT);
}

unsigned int Device::getPin() const
{
    return this->pinNum;
}

String Device::getName() const
{
    return this->name;
}

String Device::getDescription() const
{
    return this->description;
}

bool Device::getNormallyClosed() const
{
    return this->normallyClosed;
}
