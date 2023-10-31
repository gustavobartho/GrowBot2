#ifndef Device_h
#define Device_h

#include <Arduino.h>

class Device
{
private:
    unsigned int pinNum;
    uint8_t mode;

protected:
    String name;
    String description;
    bool normallyClosed;

public:
    Device(unsigned int pinNum, uint8_t mode, const String &name, const String &description, bool normallyClosed = false);
    virtual ~Device() = default;
    void setPin(unsigned int pinNum);
    unsigned int getPin() const;
    String getName() const;
    String getDescription() const;
    bool getNormallyClosed() const;
};

#endif
