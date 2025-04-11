#include <Arduino.h>
#include <WiFi.h>
#include <vector>

// Framework components
#include "DeviceManager.h"
#include "ConfigManager.h"
#include "EventBus.h"
#include "GrowBoxController.h"
#include "TelegramInteractor.h"
#include "Behaviors.h"

// Telegram credentials - you should define these in telegram_info.h
#ifndef TELEGRAM_TOKEN
#define TELEGRAM_TOKEN "YOUR_TELEGRAM_BOT_TOKEN"
#endif

#ifndef TELEGRAM_CHAT_ID
#define TELEGRAM_CHAT_ID "YOUR_CHAT_ID"
#endif

// WiFi credentials
const char *WIFI_SSID = "GUSTAVO";
const char *WIFI_PASSWORD = "06071995";

// Main controller and components
GrowBoxController *controller = nullptr;
TelegramInteractor *telegramInteractor = nullptr;

void setup()
{
    // Initialize serial communication
    Serial.begin(115200);
    Serial.println("GrowBot v2 starting...");

    // Initialize EEPROM
    ConfigManager::getInstance().begin();

    // Create the main controller
    controller = new GrowBoxController();

    // Initialize Telegram interactor
    telegramInteractor = new TelegramInteractor(TELEGRAM_TOKEN, TELEGRAM_CHAT_ID);
    telegramInteractor->setup();

    // Connect controller and interactor
    controller->setTelegramInteractor(telegramInteractor);
    telegramInteractor->setController(controller);

    // Setup controller
    controller->setup();

    Serial.println("GrowBot v2 initialized and ready!");
}

void loop()
{
    // Update the controller and interactor
    controller->update();
    telegramInteractor->update();

    // Small delay to prevent CPU overload
    delay(10);
}
