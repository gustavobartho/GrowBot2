#ifndef Actuators_h
#define Actuators_h

#include <vector>
#include <Actuator.h>
#include <Behaviors.h>

class Actuators
{
private:
    std::vector<Actuator *> actuators;

public:
    Actuators();
    ~Actuators() = default;

    std::vector<Actuator *> *getActuatorsAddr();
    void addActuator(Actuator *actuator);
    void createActuators();
    void runActuators();
};

#endif