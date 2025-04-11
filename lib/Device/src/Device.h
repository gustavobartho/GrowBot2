#ifndef Device_h
#define Device_h

#include <Arduino.h>
#include <vector>
#include <map>

/**
 * @brief Base class for all hardware components in the system
 * 
 * This class provides the common interface for all physical components,
 * including identification, lifecycle management, and basic I/O operations.
 */
class Device
{
protected:
    int pin;                    // Hardware pin number
    String name;                // Unique identifier
    String description;         // Human-readable description
    bool enabled;               // Current state
    std::map<String, String> parameters; // Device-specific parameters

public:
    /**
     * @brief Construct a new Device object
     * 
     * @param pin Hardware pin number
     * @param name Unique identifier
     * @param description Human-readable description
     */
    Device(int pin, const String &name, const String &description);

    /**
     * @brief Destroy the Device object
     */
    virtual ~Device();

    /**
     * @brief Initialize the device
     */
    virtual void setup();

    /**
     * @brief Update the device state
     */
    virtual void update();

    /**
     * @brief Enable the device
     */
    virtual void enable();

    /**
     * @brief Disable the device
     */
    virtual void disable();

    /**
     * @brief Check if the device is enabled
     * 
     * @return true if enabled
     * @return false if disabled
     */
    bool isEnabled() const;

    /**
     * @brief Get the device name
     * 
     * @return String The device name
     */
    String getName() const;

    /**
     * @brief Get the device description
     * 
     * @return String The device description
     */
    String getDescription() const;

    /**
     * @brief Get the device pin number
     * 
     * @return int The pin number
     */
    int getPin() const;

    /**
     * @brief Set a device parameter
     * 
     * @param key Parameter name
     * @param value Parameter value
     */
    void setParameter(const String &key, const String &value);

    /**
     * @brief Get a device parameter
     * 
     * @param key Parameter name
     * @return String Parameter value or empty string if not found
     */
    String getParameter(const String &key) const;

    /**
     * @brief Get all device parameters
     * 
     * @return std::map<String, String> Map of parameter names to values
     */
    std::map<String, String> getParameters() const;

    /**
     * @brief Get device status as a JSON string
     * 
     * @return String JSON representation of device status
     */
    virtual String getStatus() const;
};

#endif
