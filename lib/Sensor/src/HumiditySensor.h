#ifndef HumiditySensor_h
#define HumiditySensor_h

#include "Sensor.h"

/**
 * @brief Class for humidity sensors
 * 
 * This class extends Sensor to provide humidity-specific functionality,
 * including humidity conversion and calibration.
 */
class HumiditySensor : public Sensor
{
private:
    float calibrationOffset;    // Humidity calibration offset
    float voltageReference;     // Reference voltage for ADC
    float minHumidity;          // Minimum humidity value
    float maxHumidity;          // Maximum humidity value

public:
    /**
     * @brief Construct a new Humidity Sensor object
     * 
     * @param pin Hardware pin number
     * @param name Unique identifier
     * @param description Human-readable description
     * @param readInterval Interval between readings in milliseconds
     * @param calibrationOffset Humidity calibration offset
     * @param voltageReference Reference voltage for ADC
     * @param minHumidity Minimum humidity value
     * @param maxHumidity Maximum humidity value
     */
    HumiditySensor(int pin, const String &name, const String &description,
                  unsigned long readInterval = 1000,
                  float calibrationOffset = 0.0f,
                  float voltageReference = 3.3f,
                  float minHumidity = 0.0f,
                  float maxHumidity = 100.0f);

    /**
     * @brief Destroy the Humidity Sensor object
     */
    virtual ~HumiditySensor();

    /**
     * @brief Read and convert the humidity value
     * 
     * @return float The humidity percentage
     */
    float readValue() override;

    /**
     * @brief Get the calibration offset
     * 
     * @return float The calibration offset
     */
    float getCalibrationOffset() const;

    /**
     * @brief Set the calibration offset
     * 
     * @param offset The calibration offset
     */
    void setCalibrationOffset(float offset);

    /**
     * @brief Get the voltage reference
     * 
     * @return float The voltage reference
     */
    float getVoltageReference() const;

    /**
     * @brief Set the voltage reference
     * 
     * @param voltage The voltage reference
     */
    void setVoltageReference(float voltage);

    /**
     * @brief Get the minimum humidity
     * 
     * @return float The minimum humidity
     */
    float getMinHumidity() const;

    /**
     * @brief Set the minimum humidity
     * 
     * @param min The minimum humidity
     */
    void setMinHumidity(float min);

    /**
     * @brief Get the maximum humidity
     * 
     * @return float The maximum humidity
     */
    float getMaxHumidity() const;

    /**
     * @brief Set the maximum humidity
     * 
     * @param max The maximum humidity
     */
    void setMaxHumidity(float max);

    /**
     * @brief Get sensor status as a JSON string
     * 
     * @return String JSON representation of sensor status
     */
    String getStatus() const override;
};

#endif 