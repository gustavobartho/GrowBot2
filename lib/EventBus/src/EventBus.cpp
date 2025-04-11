#include "EventBus.h"

// Initialize static instance
EventBus *EventBus::instance = nullptr;

EventBus &EventBus::getInstance()
{
    if (instance == nullptr)
    {
        instance = new EventBus();
    }
    return *instance;
}

void EventBus::subscribe(const String &eventType, const String &subscriberId, std::function<void(const JsonDocument &)> callback)
{
    // Create subscription
    Subscription sub;
    sub.eventType = eventType;
    sub.subscriberId = subscriberId;
    sub.callback = callback;

    // Add to vector
    subscriptions.push_back(sub);
}

void EventBus::unsubscribe(const String &subscriberId, const String &eventType)
{
    // Remove subscriptions for this subscriber
    auto it = subscriptions.begin();
    while (it != subscriptions.end())
    {
        if (it->subscriberId == subscriberId && (eventType.isEmpty() || it->eventType == eventType))
        {
            it = subscriptions.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void EventBus::publish(const String &eventType, JsonDocument &data)
{
    // Notify all subscribers for this event type
    for (const auto &sub : subscriptions)
    {
        if (sub.eventType == eventType)
        {
            sub.callback(data);
        }
    }
}