#ifndef TelegramInteractor_h
#define TelegramInteractor_h

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Forward declarations
class GrowBoxController;

class TelegramInteractor
{
private:
    WiFiClientSecure client;
    UniversalTelegramBot *bot;
    String botToken;
    String chatId;
    unsigned long lastCheckTime;
    int checkInterval; // milliseconds
    GrowBoxController *controller;

    // Command handlers
    void handleStatusCommand(String chatId);
    void handleDeviceCommand(String chatId, String message);
    void handleDeviceListCommand(String chatId);
    void handleDeviceInfoCommand(String chatId, String deviceName);
    void handleDeviceStateCommand(String chatId, String deviceName, String state);
    void handleDeviceBehaviorCommand(String chatId, String deviceName, String behaviorType);
    void handleDeviceConfigCommand(String chatId, String deviceName, String config);

    // Helper method to extract values from messages
    String getValueFromMessage(String command, String message);

public:
    TelegramInteractor(const String &token, const String &id, int checkInterval = 1000);
    ~TelegramInteractor();

    void setup();
    void update();
    void setController(GrowBoxController *controller);

    // Message sending methods
    void sendMessage(const String &text);
    void sendDeviceStatus(const String &deviceName);
    void sendAllDevicesStatus();
};

#endif