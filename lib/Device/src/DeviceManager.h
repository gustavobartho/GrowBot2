#ifndef DeviceManager_h
#define DeviceManager_h

#include <vector>
#include <Arduino.h>
#include "Device.h"
#include "Actuator.h"
#include "Sensor.h"

class DeviceManager
{
private:
    static DeviceManager *instance;
    std::vector<Device *> devices;

    // Private constructor for singleton
    DeviceManager() {}

public:
    // Get singleton instance
    static DeviceManager &getInstance();

    // Delete copy constructor and assignment operator
    DeviceManager(const DeviceManager &) = delete;
    void operator=(const DeviceManager &) = delete;

    // Device management
    void registerDevice(Device *device);
    void unregisterDevice(const String &name);
    Device *getDevice(const String &name);
    const std::vector<Device *> &getAllDevices() const;

    // Get devices by type
    std::vector<Sensor *> getSensors();
    std::vector<Actuator *> getActuators();

    // Setup and update all devices
    void setupAll();
    void updateAll();

    // Cleanup - delete all devices
    void cleanup();
};

#endif