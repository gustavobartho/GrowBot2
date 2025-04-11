#include "DeviceManager.h"

// Initialize static instance
DeviceManager *DeviceManager::instance = nullptr;

DeviceManager &DeviceManager::getInstance()
{
    if (instance == nullptr)
    {
        instance = new DeviceManager();
    }
    return *instance;
}

void DeviceManager::registerDevice(Device *device)
{
    devices.push_back(device);
}

void DeviceManager::unregisterDevice(const String &name)
{
    for (auto it = devices.begin(); it != devices.end(); ++it)
    {
        if ((*it)->getName() == name)
        {
            delete *it;
            devices.erase(it);
            break;
        }
    }
}

Device *DeviceManager::getDevice(const String &name)
{
    for (Device *device : devices)
    {
        if (device->getName() == name)
        {
            return device;
        }
    }
    return nullptr;
}

const std::vector<Device *> &DeviceManager::getAllDevices() const
{
    return devices;
}

std::vector<Sensor *> DeviceManager::getSensors()
{
    std::vector<Sensor *> sensors;
    for (Device *device : devices)
    {
        Sensor *sensor = dynamic_cast<Sensor *>(device);
        if (sensor != nullptr)
        {
            sensors.push_back(sensor);
        }
    }
    return sensors;
}

std::vector<Actuator *> DeviceManager::getActuators()
{
    std::vector<Actuator *> actuators;
    for (Device *device : devices)
    {
        Actuator *actuator = dynamic_cast<Actuator *>(device);
        if (actuator != nullptr)
        {
            actuators.push_back(actuator);
        }
    }
    return actuators;
}

void DeviceManager::setupAll()
{
    for (Device *device : devices)
    {
        device->setup();
    }
}

void DeviceManager::updateAll()
{
    for (Device *device : devices)
    {
        device->update();
    }
}

void DeviceManager::cleanup()
{
    for (Device *device : devices)
    {
        delete device;
    }
    devices.clear();
}