#ifndef Interaction_h
#define Interaction_h

#include <vector>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <Actuator.h>

class Interaction
{
protected:
    WiFiClientSecure *client;

public:
    Interaction(WiFiClientSecure *client);
    ~Interaction() = default; // Add a destructor.

    virtual void handleInteractions(std::vector<Actuator *> *actuators) = 0;
};

#endif