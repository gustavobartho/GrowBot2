# Behavior Class

The `Behavior` class is a C++ class designed for use with Arduino. It provides a framework for creating different behaviors for controlling devices. This class is used in conjunction with the `Actuator` class to define how a device should behave.

## Table of Contents
- [Description](#description)
- [Usage](#usage)
- [Constructor](#constructor)
- [Public Member Functions](#public-member-functions)
- [Protected Member Functions](#protected-member-functions)
- [Example](#example)

## Description

The `Behavior` class defines various behaviors that can be associated with an actuator to control its operation. It includes settings for the behavior's type, state, and duration. The class allows customization of how a device responds to different states and duration units.

## Usage

1. Include the necessary libraries:
```cpp
#include <Arduino.h>
#include <map>
#include "Behavior.h"
```
2. Create an instance of the Behavior class with the desired type and reversed state.
3. Customize the behavior by implementing the virtual methods and using the provided public methods.

## Constructor
```cpp
Behavior(BehaviorType type, bool reversed);
```
* `type`: The type of behavior, which can be ON_OFF or STEP.
* `reversed``: A boolean indicating whether the behavior is reversed.

## Public Member Functions
```cpp
BehaviorState getState() const
```
Gets the current state of the behavior.
<br><br>

```cpp
BehaviorType getType() const
```
Gets the type of the behavior.
<br><br>

```cpp
bool getReversed() const
```
Gets the reversed state of the behavior.
<br><br>

```cpp
void setReversed(bool reversed, unsigned int pinNum)
```
Sets the reversed state of the behavior and updates the associated pin state accordingly.
<br><br>

```cpp
void deactivate(unsigned int pinNum, bool normallyClosed)
```
Deactivates the behavior and sets the associated pin state accordingly.
<br><br>

```cpp
virtual void run(unsigned int pinNum, bool normallyClosed)
```
A virtual method to implement the specific behavior for the actuator. You must provide the logic for how the actuator should behave in this method.
<br><br>

```cpp
virtual void activate(unsigned int pinNum, bool normallyClosed)
```
A virtual method to activate the behavior for the actuator. You must provide the logic for setting up the behavior in this method.
<br><br>

```cpp
virtual void setState(BehaviorState newState)
```
A virtual method to set the state of the behavior. You must provide the logic for how the state change affects the behavior.
<br><br>

## Protected Member Functions
```cpp
int getMillisecondsFromDuration(BehaviorDurationUnit durationUnit)
```
Converts a given duration unit to milliseconds. This function is used internally to handle behavior durations.
<br><br>

```cpp
unsigned long getTimeDelta(unsigned long timeNow)
```
Calculates the time elapsed since the last behavior change.
<br><br>

```cpp
void turnOn(unsigned int pinNum, bool normallyClosed)
```
Turns on the associated pin for the actuator. The behavior considers whether the pin is normally closed or reversed.
<br><br>

```cpp
void turnOff(unsigned int pinNum, bool normallyClosed)
```
Turns off the associated pin for the actuator. The behavior considers whether the pin is normally closed or reversed.

## Example

```cpp
#include <Arduino.h>
#include <map>
#include "Behavior.h"

// Create a Behavior instance
Behavior behavior(BehaviorType::ON_OFF, false);

// Set the reversed state
behavior.setReversed(true, 2);

// Activate the behavior
behavior.activate(2, false);

// Run the behavior
behavior.run(2, false);

// Set the state to ON
behavior.setState(BehaviorState::ON);
```

In this example, we create a Behavior instance, set the reversed state, activate the behavior, run the behavior, and set the state.

For more details on how to use the Behavior class, please refer to the comments and documentation in the source code.