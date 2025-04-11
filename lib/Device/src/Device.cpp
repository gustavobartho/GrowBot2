#include "Device.h"

Device::Device(int pin, const String &name, const String &description)
    : pin(pin), name(name), description(description), enabled(false)
{
}

Device::~Device()
{
}

void Device::setup()
{
    pinMode(pin, INPUT);
    enabled = false;
}

void Device::update()
{
    // Base class implementation does nothing
}

void Device::enable()
{
    enabled = true;
}

void Device::disable()
{
    enabled = false;
}

bool Device::isEnabled() const
{
    return enabled;
}

String Device::getName() const
{
    return name;
}

String Device::getDescription() const
{
    return description;
}

int Device::getPin() const
{
    return pin;
}

void Device::setParameter(const String &key, const String &value)
{
    parameters[key] = value;
}

String Device::getParameter(const String &key) const
{
    auto it = parameters.find(key);
    if (it != parameters.end())
    {
        return it->second;
    }
    return "";
}

std::map<String, String> Device::getParameters() const
{
    return parameters;
}

String Device::getStatus() const
{
    String status = "{";
    status += "\"name\":\"" + name + "\",";
    status += "\"description\":\"" + description + "\",";
    status += "\"pin\":" + String(pin) + ",";
    status += "\"enabled\":" + String(enabled ? "true" : "false") + ",";
    status += "\"parameters\":{";

    bool first = true;
    for (const auto &param : parameters)
    {
        if (!first)
        {
            status += ",";
        }
        status += "\"" + param.first + "\":\"" + param.second + "\"";
        first = false;
    }

    status += "}}";
    return status;
}
