# GrowBot Framework Architecture Documentation

## Overview

The GrowBot Framework is a modular and extensible architecture for microcontroller-based automation systems. It provides a flexible foundation for creating IoT devices with support for various sensors, actuators, behaviors, and remote control interfaces.

## Core Architecture Components

### 1. Device Management Layer

#### Device
The base class for all hardware components in the system. Provides:
- Common interface for all physical components
- Identification (name, description)
- Lifecycle management (setup, update, enable/disable)
- Pin assignment and basic I/O operations

#### Sensor
Extends Device to provide:
- Reading values from physical inputs
- Data type and unit specification
- Configurable reading intervals
- Value transformation and filtering

#### Actuator
Extends Device to provide:
- Ability to affect the physical world
- Support for different behavior patterns
- Value constraints and safety limits
- Customizable state transitions

#### DeviceManager
Central registry for all devices that provides:
- Device registration and discovery
- Lifecycle management for all devices
- Type-based filtering and querying
- Centralized setup and update cycles

### 2. Behavior Layer

#### Behavior
The base class for all behavioral patterns that can be assigned to actuators:
- State management
- Transition logic
- Configuration parameters

#### OnOffBehavior
The simplest behavior type that provides:
- Boolean state control (on/off)
- Optional timing parameters (e.g., auto-off after duration)

#### StepBehavior
Provides stepped progression through multiple states:
- Configurable number of steps
- Customizable timing for each step
- Cyclic or one-shot operation modes

#### InputBoundBehavior
Links actuator behavior to input values:
- Maps input ranges to output values
- Configurable thresholds and hysteresis
- Can be bound to sensor readings or external control signals

### 3. Configuration Layer

#### ConfigManager
Persistent storage for system configuration:
- EEPROM and SD card storage backends
- Key-value pair storage
- Type-safe getters and setters
- Default value handling
- Configuration versioning and migration

#### SDCardManager
Manages interaction with the SD card module:
- File system operations
- JSON configuration storage
- Data logging
- Firmware update handling

### 4. Communication Layer

#### EventBus
Internal message passing system:
- Publisher-subscriber pattern
- Topic-based message routing
- Support for synchronous and asynchronous messages
- Event filtering and prioritization

#### NetworkInterface
Base class for external communication channels that provides:
- Common interface for all network adapters
- Connection management
- Message formatting and parsing

#### TelegramInteractor
Extends NetworkInterface to provide:
- Telegram bot integration
- Command processing
- Status reporting
- Notification delivery

#### HTTPInterface
Extends NetworkInterface to provide:
- RESTful API endpoints
- JSON request/response handling
- WebSocket support for real-time updates
- Authentication and authorization

### 5. Controller Layer

#### GrowBoxController
System orchestration layer:
- Component integration
- High-level business logic
- Cross-component coordination
- System lifecycle management

## Data Flow and Component Interaction

1. **Setup Phase**:
   - ConfigManager loads stored configuration
   - DeviceManager initializes all registered devices
   - NetworkInterfaces establish connections
   - Controller sets up initial system state

2. **Runtime Phase**:
   - ConfigManager provides configuration values
   - Sensors read and publish data to EventBus
   - Controllers process events and decide on actions
   - Actuators execute behaviors based on controller commands
   - NetworkInterfaces relay external commands and publish status updates

3. **Configuration Phase**:
   - External commands (via Telegram, HTTP, etc.) modify system configuration
   - ConfigManager persists changes to storage
   - System components adapt to new configuration

## Command Interface Specification

The framework provides a unified command interface accessible through multiple channels (Telegram, HTTP, Serial, etc.).

### Device Management Commands

```
/device list
```
Lists all registered devices

```
/device info <device_name>
```
Shows detailed information about a specific device

```
/device create <type> <name> <pin> [description]
```
Creates a new device of the specified type
- Types: sensor, actuator
- Example: `/device create actuator light_main 25 "Main Light"`

```
/device delete <device_name>
```
Removes a device from the system

```
/device state <device_name> <state>
```
Sets the state of an actuator
- States: on, off, or a numeric value
- Example: `/device state light_main on`

### Behavior Management Commands

```
/behavior list [device_name]
```
Lists all available behaviors or behaviors assigned to a specific device

```
/behavior set <device_name> <behavior_type> [parameters]
```
Assigns a behavior to a device
- Behavior types: ON_OFF, STEP, INPUT_BOUND
- Example: `/behavior set light_main STEP steps=3 duration=3600`

```
/behavior config <device_name> <parameter>=<value>
```
Configures a behavior parameter
- Example: `/behavior config light_main duration=7200`

### System Commands

```
/status
```
Shows the overall system status

```
/config get <key>
```
Gets a configuration value

```
/config set <key> <value>
```
Sets a configuration value

```
/save
```
Forces saving current configuration to persistent storage

### SD Card Commands

```
/sd list [directory]
```
Lists files in the specified directory or root directory

```
/sd read <file_path>
```
Reads the content of a file

```
/sd write <file_path> <content>
```
Writes content to a file

```
/sd delete <file_path>
```
Deletes a file

```
/sd backup
```
Creates a backup of the current configuration

```
/sd restore <backup_name>
```
Restores configuration from a backup

## SD Card Integration

The system integrates with an SD card module for enhanced storage capabilities:

### Hardware Setup
- Uses the SPI interface for communication
- Supports standard SD and microSD cards (with adapter)
- Compatible with FAT16 and FAT32 file systems

### SDCardManager Class
The `SDCardManager` class provides:
- Initialization and card detection
- File and directory operations
- Configuration file management
- Data logging capabilities
- Backup and restore functions

### Storage Organization
The system uses an SD card for:
1. **Configuration storage** - JSON files containing system configuration
2. **Device definitions** - JSON files with device specifications
3. **Behavior definitions** - JSON files with behavior configurations
4. **Logging** - System events and sensor readings
5. **Firmware updates** - OTA update packages

### SD Card Structure
```
/config/
    system.json       - System-wide settings
    devices.json      - Device registry
    behaviors.json    - Behavior configurations
/logs/
    system_YYYY-MM-DD.log - System event logs
    sensors_YYYY-MM-DD.log - Sensor reading logs
/updates/
    firmware.bin      - Firmware update file
/backups/
    backup_YYYY-MM-DD_HHMMSS/ - Configuration backups
```

### Auto-recovery
The system implements an auto-recovery mechanism that:
- Detects configuration corruption
- Utilizes the most recent valid backup
- Creates periodic configuration snapshots
- Maintains a configuration history

## HTTP API Endpoints

The framework exposes a RESTful API for integration with external systems:

### Device Endpoints

```
GET /api/devices
```
Lists all devices

```
GET /api/devices/{device_name}
```
Gets a specific device

```
POST /api/devices
```
Creates a new device

```
PUT /api/devices/{device_name}
```
Updates a device

```
DELETE /api/devices/{device_name}
```
Deletes a device

```
PUT /api/devices/{device_name}/state
```
Sets device state
- Body: `{"state": "on"}` or `{"state": "off"}` or `{"value": 42}`

### Behavior Endpoints

```
GET /api/behaviors
```
Lists all behaviors

```
GET /api/behaviors/{device_name}
```
Gets behaviors for a specific device

```
POST /api/behaviors/{device_name}
```
Creates a behavior for a device
- Body: `{"type": "ON_OFF", "parameters": {"auto_off_seconds": 3600}}`

```
PUT /api/behaviors/{device_name}
```
Updates a behavior

### System Endpoints

```
GET /api/status
```
Gets system status

```
GET /api/config/{key}
```
Gets a configuration value

```
PUT /api/config/{key}
```
Sets a configuration value

### SD Card Endpoints

```
GET /api/sd/files[?path={directory_path}]
```
Lists files on the SD card

```
GET /api/sd/files/{file_path}
```
Gets content of a specific file

```
POST /api/sd/files/{file_path}
```
Creates or updates a file
- Body: file content

```
DELETE /api/sd/files/{file_path}
```
Deletes a file

```
POST /api/sd/backup
```
Creates a configuration backup

```
POST /api/sd/restore/{backup_name}
```
Restores from a configuration backup

## Future Extensions

### Visual Programming Interface
The architecture is designed to support a future visual programming interface that will allow:
1. Drag-and-drop device creation and configuration
2. Visual wiring of components
3. Behavior programming through visual nodes
4. Real-time monitoring and control

### Custom Component Creation
The modular design allows for easy extension with:
1. New Device types
2. Custom Behaviors
3. Additional NetworkInterfaces
4. Specialized Controllers

### Integration Points
The framework provides clear integration points for:
1. Cloud services
2. Mobile applications
3. Home automation systems
4. Machine learning models for advanced control

## Implementation Examples

### Creating a simple light controller

```cpp
// Register a light actuator
Actuator *light = new Actuator(25, "main_light", "Main Room Light");
DeviceManager::getInstance().registerDevice(light);

// Set its behavior to ON/OFF
light->setBehavior(new OnOffBehavior());

// Configure it to auto-off after 2 hours
light->getBehavior()->setParameter("auto_off_seconds", "7200");
```

### Creating a temperature-controlled fan

```cpp
// Register a temperature sensor
Sensor *tempSensor = new Sensor(32, "temp_sensor", "Temperature Sensor", "temperature", "°C");
DeviceManager::getInstance().registerDevice(tempSensor);

// Register a fan actuator
Actuator *fan = new Actuator(26, "cooling_fan", "Cooling Fan");
DeviceManager::getInstance().registerDevice(fan);

// Create an input-bound behavior that activates based on temperature
InputBoundBehavior *behavior = new InputBoundBehavior("temp_sensor");
behavior->setParameter("threshold_on", "28");
behavior->setParameter("threshold_off", "24");
behavior->setParameter("hysteresis", "2");
fan->setBehavior(behavior);
```

### Storing and loading configuration from SD card

```cpp
// Save device configuration to SD card
SDCardManager::getInstance().saveDeviceConfig();

// Load device configuration from SD card
SDCardManager::getInstance().loadDeviceConfig();

// Log sensor data to SD card
SDCardManager::getInstance().logSensorData("temp_sensor", tempSensor->getValue());
```

## Development Roadmap

1. **Core Framework**
   - Device, Sensor, and Actuator base classes
   - Basic behaviors
   - Configuration management
   - Event system

2. **Communication Interfaces**
   - Telegram bot integration
   - HTTP API
   - Serial command interface

3. **Persistent Storage**
   - SD card integration
   - Configuration persistence
   - Logging system

4. **Advanced Behaviors**
   - Time-based behaviors
   - Sensor-dependent behaviors
   - Complex state machines

5. **Visual Programming Interface**
   - Web-based UI
   - Mobile app
   - Visual programming elements

## Best Practices

1. **Naming Conventions**
   - Use descriptive names for devices
   - Follow a consistent naming pattern
   - Document the purpose of each device

2. **Resource Management**
   - Be mindful of memory usage
   - Clean up unused devices
   - Use appropriate data types

3. **Error Handling**
   - Implement timeouts for operations
   - Handle communication failures gracefully
   - Log errors for troubleshooting

4. **Security**
   - Implement authentication for remote interfaces
   - Validate all inputs
   - Use secure communication channels 