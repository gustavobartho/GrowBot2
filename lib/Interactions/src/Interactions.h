#ifndef Interactions_h
#define Interactions_h

#include <vector>

// Any new Interaction type should be included here, not in main
#include "TelegramInteraction.h"

class Interactions
{
private:
    std::vector<Interaction *> interactions;

public:
    Interactions();
    ~Interactions() = default;

    void addInteraction(Interaction *interaction);
    void handleInteractions(std::vector<Actuator *> *actuators);
};

#endif