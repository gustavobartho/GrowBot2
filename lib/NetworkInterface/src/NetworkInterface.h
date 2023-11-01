#ifndef NetworkInterface_h
#define NetworkInterface_h

#include <WiFi.h>
#include <WiFiClientSecure.h>

class NetworkInterface
{
private:
    String ssid;
    String password;

public:
    WiFiClientSecure client;

public:
    NetworkInterface();
    ~NetworkInterface() = default; // Add a destructor.

    void setDataSTA(String ssid, String password);
    bool isConnected() const;
    bool connect();
};

#endif
