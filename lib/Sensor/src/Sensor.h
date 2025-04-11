#ifndef Sensor_h
#define Sensor_h

#include "../Device/src/Device.h"

/**
 * @brief Class for sensor devices that read values from physical inputs
 * 
 * This class extends Device to provide sensor-specific functionality,
 * including reading values, data type specification, and value transformation.
 */
class Sensor : public Device
{
private:
    String dataType;    // Type of data (e.g., "temperature", "humidity")
    String unit;        // Unit of measurement (e.g., "°C", "%")
    float value;        // Current sensor value
    unsigned long lastReadTime;  // Last time the sensor was read
    unsigned long readInterval;  // Interval between readings in milliseconds

public:
    /**
     * @brief Construct a new Sensor object
     * 
     * @param pin Hardware pin number
     * @param name Unique identifier
     * @param description Human-readable description
     * @param dataType Type of data (e.g., "temperature", "humidity")
     * @param unit Unit of measurement (e.g., "°C", "%")
     * @param readInterval Interval between readings in milliseconds
     */
    Sensor(int pin, const String &name, const String &description,
           const String &dataType, const String &unit,
           unsigned long readInterval = 1000);

    /**
     * @brief Destroy the Sensor object
     */
    virtual ~Sensor();

    /**
     * @brief Initialize the sensor
     */
    void setup() override;

    /**
     * @brief Update the sensor value
     */
    void update() override;

    /**
     * @brief Read the current sensor value
     * 
     * @return float The current sensor value
     */
    virtual float readValue();

    /**
     * @brief Get the current sensor value
     * 
     * @return float The current sensor value
     */
    float getValue() const;

    /**
     * @brief Get the data type
     * 
     * @return String The data type
     */
    String getDataType() const;

    /**
     * @brief Get the unit of measurement
     * 
     * @return String The unit
     */
    String getUnit() const;

    /**
     * @brief Get the read interval
     * 
     * @return unsigned long The read interval in milliseconds
     */
    unsigned long getReadInterval() const;

    /**
     * @brief Set the read interval
     * 
     * @param interval The read interval in milliseconds
     */
    void setReadInterval(unsigned long interval);

    /**
     * @brief Get sensor status as a JSON string
     * 
     * @return String JSON representation of sensor status
     */
    String getStatus() const override;
};

#endif