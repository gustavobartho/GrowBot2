#include "NetworkInterface.h"

NetworkInterface::NetworkInterface()
{
}

void NetworkInterface::setDataSTA(String ssid, String password)
{
    // Initialize in station mode - to connect as a client to the network.
    // The other mode is access point mode where the board becomes a router.
    WiFi.mode(WIFI_STA);
    client.setInsecure();

    ssid = ssid;
    password = password;
}

bool NetworkInterface::isConnected() const
{
    return WiFi.isConnected();
}

bool NetworkInterface::connect()
{
    // Connect to the network using the provided ssid and password.
    wl_status_t status = WiFi.begin(ssid.c_str(), password.c_str());
    return isConnected();
}
