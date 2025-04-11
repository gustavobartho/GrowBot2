#ifndef TemperatureSensor_h
#define TemperatureSensor_h

#include "Sensor.h"

/**
 * @brief Class for temperature sensors
 * 
 * This class extends Sensor to provide temperature-specific functionality,
 * including temperature conversion and calibration.
 */
class TemperatureSensor : public Sensor
{
private:
    float calibrationOffset;    // Temperature calibration offset
    float voltageReference;     // Reference voltage for ADC
    float resistance;           // Reference resistance for thermistor
    float betaCoefficient;      // Beta coefficient for thermistor

public:
    /**
     * @brief Construct a new Temperature Sensor object
     * 
     * @param pin Hardware pin number
     * @param name Unique identifier
     * @param description Human-readable description
     * @param readInterval Interval between readings in milliseconds
     * @param calibrationOffset Temperature calibration offset
     * @param voltageReference Reference voltage for ADC
     * @param resistance Reference resistance for thermistor
     * @param betaCoefficient Beta coefficient for thermistor
     */
    TemperatureSensor(int pin, const String &name, const String &description,
                     unsigned long readInterval = 1000,
                     float calibrationOffset = 0.0f,
                     float voltageReference = 3.3f,
                     float resistance = 10000.0f,
                     float betaCoefficient = 3950.0f);

    /**
     * @brief Destroy the Temperature Sensor object
     */
    virtual ~TemperatureSensor();

    /**
     * @brief Read and convert the temperature value
     * 
     * @return float The temperature in Celsius
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
     * @brief Get the beta coefficient
     * 
     * @return float The beta coefficient
     */
    float getBetaCoefficient() const;

    /**
     * @brief Set the beta coefficient
     * 
     * @param beta The beta coefficient
     */
    void setBetaCoefficient(float beta);

    /**
     * @brief Get sensor status as a JSON string
     * 
     * @return String JSON representation of sensor status
     */
    String getStatus() const override;
};

#endif 