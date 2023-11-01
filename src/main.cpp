#include <vector>

// ----------------------------------------------------------------------------------------------------------

#include <NetworkInterface.h>
#include <Behaviors.h>
#include <Interactions.h>
#include <Actuator.h>

#include "personal_info.h"

// ----------------------------------------------------------------------------------------------------------

std::vector<Actuator *> actuators;
std::vector<Interaction *> interactions;
NetworkInterface network;

// ----------------------------------------------------------------------------------------------------------

void createActuators();
void createInteractions();

// ----------------------------------------------------------------------------------------------------------

void setup()
{
    network.setDataSTA(WIFI_SSID, WIFI_PASSWORD);

    if (!network.connect())
        sleep(10);

    createActuators();
    createInteractions();
}

// ------------------------------------------------

void loop()
{
    if (!network.isConnected())
        network.connect();

    for (Interaction *interaction : interactions)
        interaction->handleInteractions(&actuators);

    for (Actuator *actuator : actuators)
        actuator->runBehavior();
}

// ----------------------------------------------------------------------------------------------------------

void createActuators()
{
    Actuator *pump = new Actuator(25, "Pump", "Water pump", false);
    StepBehavior *pumpBehavior = new StepBehavior();
    pumpBehavior->addStep(10, BehaviorDurationUnit::SECONDS);
    pumpBehavior->addStep(1, BehaviorDurationUnit::MINUTES);
    pumpBehavior->addStep(10, BehaviorDurationUnit::SECONDS);
    pump->setBehavior(pumpBehavior);
    actuators.push_back(pump);

    Actuator *light = new Actuator(2, "Light", "LED Light", true);
    OnOffBehavior *lightBehavior = new OnOffBehavior();
    light->setBehavior(lightBehavior);
    actuators.push_back(light);
}

// ----------------------------------------------------------------------------------------------------------

void createInteractions()
{
    TelegramInteraction *telegramInteraction = new TelegramInteraction(TELEGRAM_TOKEN, &network.client, TELEGRAM_OWNER_ID);
    interactions.push_back(telegramInteraction);
}