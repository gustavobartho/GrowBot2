#include "GrowBoxController.h"

GrowBoxController::GrowBoxController()
    : deviceManager(DeviceManager::getInstance()),
      configManager(ConfigManager::getInstance()),
      telegramInteractor(nullptr)
{
}

GrowBoxController::~GrowBoxController()
{
    // Clean up resources if needed
}

void GrowBoxController::setup()
{
    // Initialize all registered devices
    deviceManager.setupAll();
}

void GrowBoxController::update()
{
    // Update all registered devices
    deviceManager.updateAll();
}

void GrowBoxController::setDeviceState(const String &deviceName, bool state)
{
    Device *device = deviceManager.getDevice(deviceName);
    if (device != nullptr)
    {
        if (state)
        {
            device->enable();
        }
        else
        {
            device->disable();
        }
    }
}

void GrowBoxController::setDeviceBehavior(const String &deviceName, const String &behaviorType, const String &params)
{
    Device *device = deviceManager.getDevice(deviceName);
    if (device != nullptr)
    {
        Actuator *actuator = dynamic_cast<Actuator *>(device);
        if (actuator != nullptr)
        {
            // Create appropriate behavior based on type
            Behavior *behavior = nullptr;
            if (behaviorType == "ON_OFF")
            {
                behavior = new OnOffBehavior();
            }
            else if (behaviorType == "STEP")
            {
                behavior = new StepBehavior();
            }
            else if (behaviorType == "INPUT_BOUND")
            {
                behavior = new InputBoundBehavior(params);
            }

            if (behavior != nullptr)
            {
                actuator->setBehavior(behavior);
            }
        }
    }
}

void GrowBoxController::setDeviceConfig(const String &deviceName, const String &configKey, const String &value)
{
    configManager.setValue(deviceName + "." + configKey, value);
}

bool GrowBoxController::getDeviceState(const String &deviceName) const
{
    Device *device = deviceManager.getDevice(deviceName);
    return device != nullptr ? device->isEnabled() : false;
}

String GrowBoxController::getDeviceBehavior(const String &deviceName) const
{
    Device *device = deviceManager.getDevice(deviceName);
    if (device != nullptr)
    {
        Actuator *actuator = dynamic_cast<Actuator *>(device);
        if (actuator != nullptr)
        {
            Behavior *behavior = actuator->getBehavior();
            if (behavior != nullptr)
            {
                return String(static_cast<int>(behavior->getType()));
            }
        }
    }
    return "";
}

String GrowBoxController::getDeviceConfig(const String &deviceName, const String &configKey) const
{
    return configManager.getValue(deviceName + "." + configKey);
}

void GrowBoxController::setTelegramInteractor(TelegramInteractor *interactor)
{
    telegramInteractor = interactor;
}

void GrowBoxController::handleDeviceCommand(const String &deviceName, const String &command, const String &value)
{
    if (command == "state")
    {
        setDeviceState(deviceName, value == "on");
        sendDeviceStatus(deviceName);
    }
    else if (command == "behavior")
    {
        setDeviceBehavior(deviceName, value, "");
        sendDeviceStatus(deviceName);
    }
    else if (command == "config")
    {
        // Parse config value (format: "key=value")
        int separatorIndex = value.indexOf('=');
        if (separatorIndex != -1)
        {
            String configKey = value.substring(0, separatorIndex);
            String configValue = value.substring(separatorIndex + 1);
            setDeviceConfig(deviceName, configKey, configValue);
        }
    }
}

void GrowBoxController::sendDeviceStatus(const String &deviceName)
{
    if (telegramInteractor == nullptr)
        return;

    Device *device = deviceManager.getDevice(deviceName);
    if (device != nullptr)
    {
        String message = "Status do dispositivo " + deviceName + ":\n";
        message += "- Estado: " + String(device->isEnabled() ? "ligado" : "desligado") + "\n";
        message += "- Comportamento: " + getDeviceBehavior(deviceName) + "\n";

        // Add any device-specific status information
        if (Actuator *actuator = dynamic_cast<Actuator *>(device))
        {
            Behavior *behavior = actuator->getBehavior();
            if (behavior != nullptr)
            {
                message += "- Estado do comportamento: " + String(static_cast<int>(behavior->getState())) + "\n";
            }
        }

        telegramInteractor->sendMessage(message);
    }
}

void GrowBoxController::sendAllDevicesStatus()
{
    if (telegramInteractor == nullptr)
        return;

    String message = "Status de todos os dispositivos:\n\n";

    for (Device *device : deviceManager.getAllDevices())
    {
        message += device->getName() + ":\n";
        message += "- Estado: " + String(device->isEnabled() ? "ligado" : "desligado") + "\n";
        message += "- Comportamento: " + getDeviceBehavior(device->getName()) + "\n";
        message += "\n";
    }

    telegramInteractor->sendMessage(message);
}