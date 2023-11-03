#include <NetworkInterface.h>
#include <Interactions.h>
#include <Actuators.h>

#include "personal_info.h"

// ----------------------------------------------------------------------------------------------------------

Actuators actuators;
Interactions interactions;
NetworkInterface network;

// ----------------------------------------------------------------------------------------------------------

void setup()
{
    network.setDataSTA(WIFI_SSID, WIFI_PASSWORD);
    if (!network.connect())
        sleep(10);

    interactions.createInteractions(&network.client);

    actuators.createActuators();
}

// ------------------------------------------------

void loop()
{
    if (!network.isConnected())
        network.connect();

    interactions.handleInteractions(actuators.getActuatorsAddr());

    actuators.runActuators();
}

// ----------------------------------------------------------------------------------------------------------
