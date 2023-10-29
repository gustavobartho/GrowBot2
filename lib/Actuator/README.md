# Actuator Class

The `Actuator` class is a C++ class designed for use with Arduino and provides a mechanism for controlling devices with various behaviors. It is a part of a larger system for managing and controlling devices.

## Table of Contents
- [Description](#description)
- [Usage](#usage)
- [Constructor](#constructor)
- [Public Member Functions](#public-member-functions)
- [Example](#example)

## Description

The `Actuator` class extends the `Device` class and is used to control devices with different behaviors. It allows you to set behaviors and activate them, enabling control of devices connected to an Arduino.

## Usage

1. Include the necessary libraries:
```cpp
#include <Arduino.h>
#include <vector>
#include "Device.h"
#include "Behavior.h"
#include "Actuator.h"
```
2. Create an instance of the Actuator class with the desired pin number, name, description, and whether it is normally closed or not.
3. Set a behavior using the setBehavior method and activate it.
4. Run the behavior using the runBehavior method.
5. Set the behavior state using the setBehaviorState method.

## Constructor

```cpp
Actuator(unsigned int pinNum, String name, String description, bool normallyClosed);
```
* **pinNum**: The Arduino pin number to which the actuator is connected.
* **name**: A string representing the name of the actuator.
* **description**: A string describing the actuator.
* **normallyClosed**: A boolean indicating whether the actuator is normally closed or not.

## Public Member Functions

```cpp
void setBehavior(Behavior *behavior)
```
Sets the behavior of the actuator.
* `behavior`: A pointer to a Behavior object.
<br><br>

```cpp
void runBehavior()
```
Runs the current behavior of the actuator.
<br><br>

```cpp
void setBehaviorState(BehaviorState state)
```
Sets the state of the current behavior of the actuator.
* `state`: The new behavior state. 

##Example

```cpp
#include <Arduino.h>
#include <vector>
#include "Device.h"
#include "Behavior.h"
#include "Actuator.h"

// Create an Actuator instance
Actuator actuator(2, "My Actuator", "Description of my actuator", false);

// Create a behavior instance
Behavior behavior;

// Set the behavior
actuator.setBehavior(&behavior);

// Run the behavior
actuator.runBehavior();

// Set the behavior state
actuator.setBehaviorState(BehaviorState::ON);
```

In this example, we create an Actuator instance, set a behavior, run the behavior, and set the behavior state.

For more details on how to use the Actuator class, please refer to the comments and documentation in the source code.