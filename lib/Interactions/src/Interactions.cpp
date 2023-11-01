#include "Interactions.h"

Interactions::Interactions() {}

void Interactions::addInteraction(Interaction *interaction)
{
    interactions.push_back(interaction);
}

void Interactions::handleInteractions(std::vector<Actuator *> *actuators)
{
    for (Interaction *interaction : interactions)
        interaction->handleInteractions(actuators);
}