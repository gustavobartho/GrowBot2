#include <vector>

// ----------------------------------------------------------------------------------------------------------

#include <NetworkInterface.h>
#include <Behaviors.h>
#include <Actuator.h>

// ----------------------------------------------------------------------------------------------------------

std::vector<Actuator *> actuators;
NetworkInterface network;

// ----------------------------------------------------------------------------------------------------------

void createActuators();

// ----------------------------------------------------------------------------------------------------------

void setup()
{
    createActuators();

    network.setDataSTA("GUSTAVO", "06071995");

    if (!network.connect())
        sleep(10);
}

// ------------------------------------------------

void loop()
{
    if (!network.isConnected())
        network.connect();

    for (Actuator *actuator : actuators)
        actuator->runBehavior();
}

// ----------------------------------------------------------------------------------------------------------

void createActuators()
{
    Actuator *pump = new Actuator(2, "Pump", "Water pump", false);
    StepBehavior *pumpBehavior = new StepBehavior();
    pumpBehavior->addStep(15, BehaviorDurationUnit::SECONDS);
    pumpBehavior->addStep(1, BehaviorDurationUnit::MINUTES);
    pumpBehavior->addStep(30, BehaviorDurationUnit::SECONDS);
    pumpBehavior->addStep(15, BehaviorDurationUnit::SECONDS);    
    pump->setBehavior(pumpBehavior);
    actuators.push_back(pump);
}

// ----------------------------------------------------------------------------------------------------------
