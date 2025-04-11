#ifndef ConfigManager_h
#define ConfigManager_h

#include <Arduino.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

// Configuration constants
constexpr size_t EEPROM_SIZE = 512;
constexpr int CONFIG_VERSION = 1;

class ConfigManager
{
private:
    static ConfigManager *instance;

    // EEPROM address constants
    static const int VERSION_ADDRESS = 0;
    static const int IRRIGATION_INTERVAL_ADDRESS = 4;
    static const int AUTO_IRRIGATION_ADDRESS = 8;
    static const int IRRIGATION_TIME_ADDRESS = 12;
    static const int PHOTOPERIOD_ADDRESS = 16;
    static const int POWER_SAVING_ADDRESS = 20;

    // Configuration values
    int irrigationIntervalInDays;
    int irrigationTimeInSeconds;
    int photoperiod;
    bool autoIrrigationEnabled;
    bool powerSavingEnabled;

    // Private constructor for singleton
    ConfigManager();

public:
    // Get singleton instance
    static ConfigManager &getInstance();

    // Delete copy constructor and assignment operator
    ConfigManager(const ConfigManager &) = delete;
    void operator=(const ConfigManager &) = delete;

    // Initialize EEPROM
    void begin();

    // Load/save configuration
    void load();
    void save();

    // Configuration getters
    int getIrrigationInterval() const;
    int getIrrigationTime() const;
    int getPhotoperiod() const;
    bool isAutoIrrigationEnabled() const;
    bool isPowerSavingEnabled() const;

    // Configuration setters
    void setIrrigationInterval(int days);
    void setIrrigationTime(int seconds);
    void setPhotoperiod(int period);
    void setAutoIrrigationEnabled(bool enabled);
    void setPowerSavingEnabled(bool enabled);

    // Reset to default values
    void resetToDefaults();

private:
    // Write values to EEPROM
    void writeInt(int address, int value);
    void writeBool(int address, bool value);

    // Read values from EEPROM
    int readInt(int address);
    bool readBool(int address);
};

#endif