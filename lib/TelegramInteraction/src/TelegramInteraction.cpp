#include "TelegramInteraction.h"

TelegramInteraction::TelegramInteraction(WiFiClientSecure *client)
    : Interaction(client),
      telegramBot(TELEGRAM_TOKEN, *this->client),
      ownerId(TELEGRAM_OWNER_ID)
{
    telegramBot.sendMessage(ownerId, "-- Board activated --");
}

void TelegramInteraction::handleInteractions(std::vector<Actuator *> *actuators)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        int numNewMessages = telegramBot.getUpdates(telegramBot.last_message_received + 1);

        if (numNewMessages > 0)
        {
            for (int i = 0; i < numNewMessages; i++)
            {
                String comando = telegramBot.messages[i].text;
                String chatId = telegramBot.messages[i].chat_id;

                if (chatId == ownerId)
                {
                    // general actuators and sensors status
                    if (comando.equalsIgnoreCase(commands.status))
                    {
                        sendStatusMessage(chatId, actuators);
                    }
                    /*
                    else if (comando.equalsIgnoreCase(commands.veg) && photoperiod != VEG) // Muda para o fotoperíodo de vegetativo
                    {
                        changePhotoperiod(chatId, VEG);
                        sendLightStatusMessage(chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.flor) && photoperiod != FLOR) // Muda para o fotoperíodo de floração
                    {
                        changePhotoperiod(chatId, FLOR);
                        sendLightStatusMessage(chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.ger) && photoperiod != GER) // Muda para o fotoperíodo de germinação
                    {
                        changePhotoperiod(chatId, GER);
                        sendLightStatusMessage(chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.light)) // Envia a mensagem com o status da iluminação
                    {
                        sendLightStatusMessage(chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.lightOn) && currentLightStep > 2) // Liga a luz
                    {
                        changeLightState(ON);
                        sendLightStatusMessage(chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.lightOff) && currentLightStep < 3) // Desliga a luz
                    {
                        changeLightState(OFF);
                        sendLightStatusMessage(chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.ventilation)) // Envia a mensagem com o status da ventilação
                    {
                        sendVentilationStatusMessage(chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.ventilationOn) && ventilationStatus != ON) // Liga a ventilação
                    {
                        changeVentilationStatus(ON);
                        sendVentilationStatusMessage(chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.ventilationOff) && ventilationStatus != OFF) // Desliga a a ventilação
                    {
                        changeVentilationStatus(OFF);
                        sendVentilationStatusMessage(chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.powerSavingOn) && powerSavingStatus != ON) // Liga o modo de economia de energia
                    {
                        changePowerSavingStatus(ON);
                        sendPowerSavingStatus(chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.powerSavingOff) && powerSavingStatus != OFF) // Desliga o modo de economia de energia
                    {
                        changePowerSavingStatus(OFF);
                        sendPowerSavingStatus(chatId);
                    }
                    */
                }
            }
        }
        return;
    }
}

void TelegramInteraction::sendStatusMessage(String chatId, std::vector<Actuator *> *actuators)
{
    String message = "Status:\n\n";

    // Actuators Status
    message += "ACTUATORS\n\n";
    getActuatorsStatusMessage(chatId, actuators, &message);
    message += "\n";

    telegramBot.sendMessage(chatId, message);
}

void TelegramInteraction::getActuatorsStatusMessage(String chatId, std::vector<Actuator *> *actuators, String *message)
{
    for (Actuator *actuator : *actuators)
    {
        // Set behavior general data
        BehaviorStatusData behaviorData = actuator->behavior->getStatusData();
        *message += "Pin: " + String(actuator->getPin()) + "\n";
        *message += "- Name: " + String(actuator->getName()) + "\n";
        *message += "- Description: " + String(actuator->getDescription()) + "\n";
        *message += "- " + String(actuator->getNormallyClosed() ? "Normally closed" : "Normally open") + "\n";
        *message += "- Behavior type: " + String(behaviorData.typeFormatted) + "\n";
        *message += "- Behavior state: " + String(behaviorData.stateFormatted) + "\n";
        *message += "- Behavior " + String(behaviorData.reversed ? "" : "not ") + "reversed\n";
        *message += "- " + behaviorData.timeSinceLastChangeFormatted + " since last change\n";

        // Set specific Behavior data
        if (actuator->behavior->getType() == BehaviorType::STEP)
        {
            *message += "- Behavior step: " + String(behaviorData.step) + '\n';
        }
    }
}