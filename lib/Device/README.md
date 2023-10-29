# Device Class

The `Device` class is a fundamental C++ class designed for use with Arduino. It serves as a base class for devices and provides essential functionality for managing device-specific attributes and operations. Other device-specific classes can inherit from this class to build on its functionality.

## Table of Contents
- [Description](#description)
- [Usage](#usage)
- [Constructor](#constructor)
- [Public Member Functions](#public-member-functions)

## Description

The `Device` class encapsulates common attributes and methods for devices connected to an Arduino. It is intended to simplify the development of device-specific classes by providing a common structure for managing device-related details. This class is commonly used as a base class for various devices, allowing developers to extend it with device-specific features.

## Usage

1. Include the necessary libraries:
```cpp
#include <Arduino.h>
#include "Device.h"
```
2. Create a new class representing a specific device (e.g., an actuator or sensor).
3. Inherit from the Device class to leverage its common attributes and methods.
4. Implement device-specific logic and behavior in the derived class.

## Constructor

```cpp
Device(unsigned int pinNum, uint8_t mode, const String &name, const String &description, bool normallyClosed = false);
```
* `pinNum`: The Arduino pin number to which the device is`connected.
* `mode`: The mode of the pin (e.g., INPUT, OUTPUT).
* `name`: A string representing the name of the device.
* `description`: A string describing the device.
* `normallyClosed`: A boolean indicating whether the device is normally closed or not (default is false).

## Public Member Functions
```cpp
void setPin(unsigned int pinNum)
```
Sets or changes the pin number associated with the device.
<br><br>
```cpp
unsigned int getPin() const
```
Gets the current pin number associated with the device.
