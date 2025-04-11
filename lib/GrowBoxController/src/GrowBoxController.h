#ifndef GrowBoxController_h
#define GrowBoxController_h

#include <Arduino.h>
#include "DeviceManager.h"
#include "ConfigManager.h"
#include "TelegramInteractor.h"

// Constants from main_old.cpp
#define ONE_HOUR 3600000
#define OFF 0
#define ON 1
#define GER 11
#define VEG 22
#define FLOR 33

class GrowBoxController
{
private:
    // Components
    TelegramInteractor *telegramInteractor;
    DeviceManager &deviceManager;
    ConfigManager &configManager;

    // Helper methods
    void handleDeviceCommand(const String &deviceName, const String &command, const String &value = "");
    void sendDeviceStatus(const String &deviceName);
    void sendAllDevicesStatus();

public:
    GrowBoxController();
    ~GrowBoxController();

    void setup();
    void update();

    // Device control methods
    void setDeviceState(const String &deviceName, bool state);
    void setDeviceBehavior(const String &deviceName, const String &behaviorType, const String &params);
    void setDeviceConfig(const String &deviceName, const String &configKey, const String &value);

    // Status methods
    bool getDeviceState(const String &deviceName) const;
    String getDeviceBehavior(const String &deviceName) const;
    String getDeviceConfig(const String &deviceName, const String &configKey) const;

    // Set telegram interactor
    void setTelegramInteractor(TelegramInteractor *interactor);
};

#endif