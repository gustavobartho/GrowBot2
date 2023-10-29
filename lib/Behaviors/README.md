# Behaviors Class

The `Behaviors` class is a header file designed to consolidate various behavior classes for ease of use. This header file simplifies the process of importing and managing different behavior classes within your project. By including this single file in your main program, you can access all available behaviors without the need to import individual behavior headers separately.

## Table of Contents
- [Description](#description)
- [Usage](#usage)
- [Included Behavior Classes](#included-behavior-classes)

## Description

In larger projects or systems where you need to work with multiple behavior types, it can become cumbersome to include each individual behavior header. The `Behaviors` class simplifies this by grouping and managing all the necessary behavior classes in one place.

## Usage

1. Include the `Behaviors` header file in your main program.

2. Use the included behavior classes as needed within your program.

## Included Behavior Classes

### `OnOffBehavior`
- A class representing the on/off type of behavior.

### `StepBehavior`
- A class representing a step-based behavior.

For detailed information on each behavior class and how to use them, please refer to their respective documentation. Importing the `Behaviors` header file provides access to all these classes in your project.

### Example

```cpp
#include "Behaviors.h"

// Create instances of behavior classes
OnOffBehavior onOffBehavior;
StepBehavior stepBehavior;

// Use the behavior classes in your project
onOffBehavior.activate(2, false);
onOffBehavior.run(2, false);

stepBehavior.activate(3, true);
stepBehavior.run(3, true);
``````

In this example, we include the Behaviors header file, create instances of behavior classes, and demonstrate how to use them in a project. By importing Behaviors.h, you can access all the included behavior classes seamlessly.