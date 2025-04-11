#ifndef LightSensor_h
#define LightSensor_h

#include "Sensor.h"

/**
 * @brief Class for light sensors
 * 
 * This class extends Sensor to provide light-specific functionality,
 * including light intensity conversion and calibration.
 */
class LightSensor : public Sensor
{
private:
    float calibrationOffset;    // Light calibration offset
    float voltageReference;     // Reference voltage for ADC
    float minLux;              // Minimum light intensity
    float maxLux;              // Maximum light intensity
    float resistance;          // Reference resistance for LDR

public:
    /**
     * @brief Construct a new Light Sensor object
     * 
     * @param pin Hardware pin number
     * @param name Unique identifier
     * @param description Human-readable description
     * @param readInterval Interval between readings in milliseconds
     * @param calibrationOffset Light calibration offset
     * @param voltageReference Reference voltage for ADC
     * @param minLux Minimum light intensity
     * @param maxLux Maximum light intensity
     * @param resistance Reference resistance for LDR
     */
    LightSensor(int pin, const String &name, const String &description,
               unsigned long readInterval = 1000,
               float calibrationOffset = 0.0f,
               float voltageReference = 3.3f,
               float minLux = 0.0f,
               float maxLux = 100000.0f,
               float resistance = 10000.0f);

    /**
     * @brief Destroy the Light Sensor object
     */
    virtual ~LightSensor();

    /**
     * @brief Read and convert the light intensity value
     * 
     * @return float The light intensity in lux
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
     * @brief Get the minimum light intensity
     * 
     * @return float The minimum light intensity
     */
    float getMinLux() const;

    /**
     * @brief Set the minimum light intensity
     * 
     * @param min The minimum light intensity
     */
    void setMinLux(float min);

    /**
     * @brief Get the maximum light intensity
     * 
     * @return float The maximum light intensity
     */
    float getMaxLux() const;

    /**
     * @brief Set the maximum light intensity
     * 
     * @param max The maximum light intensity
     */
    void setMaxLux(float max);

    /**
     * @brief Get the reference resistance
     * 
     * @return float The reference resistance
     */
    float getResistance() const;

    /**
     * @brief Set the reference resistance
     * 
     * @param resistance The reference resistance
     */
    void setResistance(float resistance);

    /**
     * @brief Get sensor status as a JSON string
     * 
     * @return String JSON representation of sensor status
     */
    String getStatus() const override;
};

#endif 