#include "TelegramInteraction.h"

TelegramInteraction::TelegramInteraction(String token, WiFiClientSecure *client) : Interaction(client), telegramBot(token, *this->client) {}

void TelegramInteraction::handleInputInteraction(std::vector<Actuator *> *actuators)
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

                if (chatId == TELEGRAM_OWNER_ID)
                {
                    // general actuators and sensors status
                    if (comando.equalsIgnoreCase(commands.status))
                    {
                        sendStatusInfo(chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.photoperiod)) // Informa o fotoperíodo
                    {
                        telegramBot.sendMessage(chatId, getLightCycleName(photoperiod));
                    }
                    else if (comando.equalsIgnoreCase(commands.irrigation)) // Informa o status da irrigação
                    {
                        sendIrrigationStatusMessage(chatId, true, autoIrrigationStatus == ON);
                    }
                    else if (comando.equalsIgnoreCase(commands.irrigate)) // Realiza uma irrigação
                    {
                        irrigate(chatId);
                        sendIrrigationStatusMessage(chatId, false);
                    }
                    else if (comando.equalsIgnoreCase(commands.irrigated)) // Registra uma irrigação manual
                    {
                        registerIrrigation(chatId);
                        sendIrrigationStatusMessage(chatId, false);
                    }
                    else if (comando.indexOf(commands.irrigationInterval) >= 0) // Muda o intervalo entre irrigações
                    {
                        updateIrrigationInterval(comando, chatId);
                    }
                    else if (comando.indexOf(commands.irrigationTime) >= 0) // Muda o tempo de uma irrigação
                    {
                        updateIrrigationTime(comando, chatId);
                    }
                    else if (comando.equalsIgnoreCase(commands.autoIrrigationOn) && autoIrrigationStatus != ON) // Liga a auto-irrigação
                    {
                        changeAutoIrrigationState(chatId, ON);
                        sendIrrigationStatusMessage(chatId, true, autoIrrigationStatus == ON);
                    }
                    else if (comando.equalsIgnoreCase(commands.autoIrrigationOff) && autoIrrigationStatus != OFF) // Desliga a auto-irrigação
                    {
                        changeAutoIrrigationState(chatId, OFF);
                        sendIrrigationStatusMessage(chatId, true, autoIrrigationStatus == ON);
                    }
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
                }
            }
        }
        return;
    }
}

void TelegramInteraction::sendStatusMessage(String chatId, std::vector<Actuator *> *actuators)
{
    String message = "Status:\n\n";

    // light status
    message += "ACTUATORS\n";
    for (Actuator *actuator : *actuators)
    {
        message += "Pin: " + String(actuator->getPin()) + "\n";
        message += "- Name: " + String(actuator->getName()) + "\n";
        message += "- Description: " + String(actuator->getDescription()) + "\n";
        message += "- " + String(actuator->getNormallyClosed() ? "Normally closed" : "Normally open") + "\n";
        message += "- Behavior type: " + String(actuator->behavior->getTypeFormatted()) + "\n";
        message += "- Behavior state: " + String(actuator->behavior->getStateFormatted()) + "\n";
        message += "- Behavior " + String(actuator->behavior->getReversed() ? "" : "not ") + "reversed\n";
        message += "- " + actuator->behavior->getTimeSinceLastChangeFormatted() + "since last change\n";
        // TODO: Create Function to Add specific type status info

        message += "\n";
    }

    telegramBot.sendMessage(chatId, message);
}