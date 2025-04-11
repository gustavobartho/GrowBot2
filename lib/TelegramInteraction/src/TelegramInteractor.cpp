#include "TelegramInteractor.h"
#include "GrowBoxController.h"

#ifdef TELEGRAM_TOKEN
#define TOKEN TELEGRAM_TOKEN
#else
#define TOKEN "YOUR_TELEGRAM_BOT_TOKEN"
#endif

#ifdef TELEGRAM_CHAT_ID
#define CHAT_ID TELEGRAM_CHAT_ID
#else
#define CHAT_ID "YOUR_CHAT_ID"
#endif

// Command strings
const String CMD_STATUS = "/status";
const String CMD_DEVICE = "/device";
const String CMD_DEVICE_LIST = "/devices";
const String CMD_DEVICE_INFO = "/info";
const String CMD_DEVICE_STATE = "/state";
const String CMD_DEVICE_BEHAVIOR = "/behavior";
const String CMD_DEVICE_CONFIG = "/config";

TelegramInteractor::TelegramInteractor(const String &token, const String &id, int checkInterval)
    : botToken(token), chatId(id), checkInterval(checkInterval), lastCheckTime(0), controller(nullptr)
{
    client.setInsecure();
    bot = new UniversalTelegramBot(botToken, client);
}

TelegramInteractor::~TelegramInteractor()
{
    if (bot)
    {
        delete bot;
    }
}

void TelegramInteractor::setup()
{
    lastCheckTime = millis();
}

void TelegramInteractor::update()
{
    if (controller == nullptr)
        return;

    unsigned long currentTime = millis();

    // Check for new messages every checkInterval milliseconds
    if (currentTime - lastCheckTime > checkInterval)
    {
        int numNewMessages = bot->getUpdates(bot->last_message_received + 1);

        for (int i = 0; i < numNewMessages; i++)
        {
            String chatId = bot->messages[i].chat_id;
            String text = bot->messages[i].text;

            // Convert to lowercase for case-insensitive comparison
            String command = text.toLowerCase();

            // Handle commands
            if (command.equals(CMD_STATUS))
            {
                handleStatusCommand(chatId);
            }
            else if (command.startsWith(CMD_DEVICE))
            {
                handleDeviceCommand(chatId, command);
            }
            else if (command.equals(CMD_DEVICE_LIST))
            {
                handleDeviceListCommand(chatId);
            }
        }

        lastCheckTime = currentTime;
    }
}

void TelegramInteractor::setController(GrowBoxController *controller)
{
    this->controller = controller;
}

void TelegramInteractor::sendMessage(const String &text)
{
    bot->sendMessage(chatId, text);
}

void TelegramInteractor::sendDeviceStatus(const String &deviceName)
{
    if (controller != nullptr)
    {
        controller->sendDeviceStatus(deviceName);
    }
}

void TelegramInteractor::sendAllDevicesStatus()
{
    if (controller != nullptr)
    {
        controller->sendAllDevicesStatus();
    }
}

// Command Handlers

void TelegramInteractor::handleStatusCommand(String chatId)
{
    sendAllDevicesStatus();
}

void TelegramInteractor::handleDeviceCommand(String chatId, String message)
{
    // Format: /device <deviceName> <command> [value]
    // Example: /device light state on
    // Example: /device pump behavior ON_OFF
    // Example: /device sensor config interval=1000

    // Split message into parts
    int firstSpace = message.indexOf(' ', CMD_DEVICE.length());
    if (firstSpace == -1)
    {
        sendMessage("Uso: /device <nome_do_dispositivo> <comando> [valor]\n\nComandos disponíveis:\n- state on/off\n- behavior ON_OFF/STEP/INPUT_BOUND\n- config key=value");
        return;
    }

    int secondSpace = message.indexOf(' ', firstSpace + 1);
    if (secondSpace == -1)
    {
        sendMessage("Comando inválido. Use: /device <nome_do_dispositivo> <comando> [valor]");
        return;
    }

    String deviceName = message.substring(firstSpace + 1, secondSpace);
    String command = message.substring(secondSpace + 1);
    String value = "";

    // Check if there's a value
    int valueSpace = command.indexOf(' ');
    if (valueSpace != -1)
    {
        value = command.substring(valueSpace + 1);
        command = command.substring(0, valueSpace);
    }

    // Handle specific commands
    if (command.equals("state"))
    {
        handleDeviceStateCommand(chatId, deviceName, value);
    }
    else if (command.equals("behavior"))
    {
        handleDeviceBehaviorCommand(chatId, deviceName, value);
    }
    else if (command.equals("config"))
    {
        handleDeviceConfigCommand(chatId, deviceName, value);
    }
    else if (command.equals("info"))
    {
        handleDeviceInfoCommand(chatId, deviceName);
    }
    else
    {
        sendMessage("Comando inválido. Comandos disponíveis:\n- state on/off\n- behavior ON_OFF/STEP/INPUT_BOUND\n- config key=value\n- info");
    }
}

void TelegramInteractor::handleDeviceListCommand(String chatId)
{
    if (controller == nullptr)
        return;

    String message = "Dispositivos disponíveis:\n\n";
    // Note: This would need to be implemented in DeviceManager
    // for (const String &deviceName : controller->getDeviceNames())
    // {
    //     message += "- " + deviceName + "\n";
    // }
    sendMessage(message);
}

void TelegramInteractor::handleDeviceInfoCommand(String chatId, String deviceName)
{
    sendDeviceStatus(deviceName);
}

void TelegramInteractor::handleDeviceStateCommand(String chatId, String deviceName, String state)
{
    if (controller == nullptr)
        return;

    if (state == "on" || state == "off")
    {
        controller->setDeviceState(deviceName, state == "on");
        sendDeviceStatus(deviceName);
    }
    else
    {
        sendMessage("Estado inválido. Use 'on' ou 'off'.");
    }
}

void TelegramInteractor::handleDeviceBehaviorCommand(String chatId, String deviceName, String behaviorType)
{
    if (controller == nullptr)
        return;

    if (behaviorType == "ON_OFF" || behaviorType == "STEP" || behaviorType == "INPUT_BOUND")
    {
        controller->setDeviceBehavior(deviceName, behaviorType, "");
        sendDeviceStatus(deviceName);
    }
    else
    {
        sendMessage("Tipo de comportamento inválido. Use:\n- ON_OFF\n- STEP\n- INPUT_BOUND");
    }
}

void TelegramInteractor::handleDeviceConfigCommand(String chatId, String deviceName, String config)
{
    if (controller == nullptr)
        return;

    if (config.indexOf('=') != -1)
    {
        controller->setDeviceConfig(deviceName, config);
        sendDeviceStatus(deviceName);
    }
    else
    {
        sendMessage("Formato de configuração inválido. Use: key=value");
    }
}

String TelegramInteractor::getValueFromMessage(String command, String message)
{
    String formattedCommand = command + " ";
    if (message.length() <= formattedCommand.length())
    {
        return "";
    }

    int index = message.indexOf(formattedCommand);
    if (index < 0)
    {
        return "";
    }

    return message.substring(formattedCommand.length());
}