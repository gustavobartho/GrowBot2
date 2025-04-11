#ifndef EventBus_h
#define EventBus_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>
#include <vector>

class EventBus
{
private:
    struct Subscription
    {
        String eventType;
        String subscriberId;
        std::function<void(const JsonDocument &)> callback;
    };

    static EventBus *instance;
    std::vector<Subscription> subscriptions;

    // Private constructor for singleton
    EventBus() {}

public:
    // Get singleton instance
    static EventBus &getInstance();

    // Delete copy constructor and assignment operator
    EventBus(const EventBus &) = delete;
    void operator=(const EventBus &) = delete;

    // Event subscription and publishing
    void subscribe(const String &eventType, const String &subscriberId, std::function<void(const JsonDocument &)> callback);
    void unsubscribe(const String &subscriberId, const String &eventType = "");
    void publish(const String &eventType, JsonDocument &data);
};

#endif