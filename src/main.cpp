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

// ----------------------------------------------------------------------------------------------------------

void setup()
{
    createActuators();

    network.setDataSTA("SSID", "PASSWORD");

    if (!network.connect())
        sleep(10);
}

// ------------------------------------------------

void loop()
{
    if (!network.isConnected())
        network.connect();

    for (Interaction *interaction : interactions)
        interaction->handleInputInteraction(&actuators);

    for (Actuator *actuator : actuators)
        actuator->runBehavior();
}

// ----------------------------------------------------------------------------------------------------------

void createActuators()
{
    Actuator *pump = new Actuator(25, "Pump", "Water pump", false);
    StepBehavior *pumpBehavior = new StepBehavior();
    pumpBehavior->addStep(3, BehaviorDurationUnit::MINUTES);
    pumpBehavior->addStep(5, BehaviorDurationUnit::MINUTES);
    pumpBehavior->addStep(3, BehaviorDurationUnit::MINUTES);
    pump->setBehavior(pumpBehavior);
    actuators.push_back(pump);
}

// ----------------------------------------------------------------------------------------------------------
