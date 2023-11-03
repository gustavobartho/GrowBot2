#include "Actuators.h"

Actuators::Actuators() {}

// ----------------------------------------------------------------------------------------------------------

void Actuators::createActuators()
{
    Actuator *pump = new Actuator(25, "Pump", "Water pump", false);
    StepBehavior *pumpBehavior = new StepBehavior();
    pumpBehavior->addStep(10, BehaviorDurationUnit::SECONDS);
    pumpBehavior->addStep(1, BehaviorDurationUnit::MINUTES);
    pumpBehavior->addStep(10, BehaviorDurationUnit::SECONDS);
    pump->setBehavior(pumpBehavior);
    addActuator(pump);

    Actuator *light = new Actuator(2, "Light", "LED Light", true);
    OnOffBehavior *lightBehavior = new OnOffBehavior();
    light->setBehavior(lightBehavior);
    addActuator(light);
}

// ----------------------------------------------------------------------------------------------------------

void Actuators::runActuators()
{
    for (Actuator *actuator : actuators)
        actuator->runBehavior();
}

// ----------------------------------------------------------------------------------------------------------

void Actuators::addActuator(Actuator *actuator)
{
    actuators.push_back(actuator);
}

// ----------------------------------------------------------------------------------------------------------

std::vector<Actuator *> *Actuators::getActuatorsAddr()
{
    return &actuators;
}

// ----------------------------------------------------------------------------------------------------------
