#ifndef TelegramInteraction_h
#define TelegramInteraction_h

#include <UniversalTelegramBot.h>
#include "Interaction.h"

struct TelegramCommands
{
    String status = "/status";
    String light = "/luz";
    String lightOn = "/ligaluz";
    String lightOff = "/desligaluz";
    String photoperiod = "/ciclo";
    String ger = "/ger";
    String veg = "/veg";
    String flor = "/flor";
    String powerSavingOn = "/ligaeconomia";
    String powerSavingOff = "/desligaeconomia";
    String irrigation = "/irrigacao";
    String irrigate = "/irrigar";
    String irrigated = "/irrigado";
    String autoIrrigationOn = "/ligaautoirrigacao";
    String autoIrrigationOff = "/desligaautoirrigacao";
    String irrigationInterval = "/intervaloirrigacao";
    String irrigationTime = "/tempoirrigacao";
    String ventilation = "/ventilacao";
    String ventilationOn = "/ligaventilacao";
    String ventilationOff = "/desligaventilacao";
};

class TelegramInteraction : public Interaction
{
private:
    UniversalTelegramBot telegramBot;
    TelegramCommands commands;
    String ownerId;

public:
    TelegramInteraction(String token, WiFiClientSecure *client, String ownerId);
    ~TelegramInteraction() = default;

    void handleInputInteraction(std::vector<Actuator *> *actuators) override;

private:
    void sendStatusMessage(String chatId, std::vector<Actuator *> *actuators);
    void getActuatorSpecificStatusData(Actuator *actuator, String *message, BehaviorStatusData *behaviorData);
};

#endif