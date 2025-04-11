#include "ConfigManager.h"

// Default values
constexpr int DEFAULT_IRRIGATION_INTERVAL = 3; // 3 days
constexpr int DEFAULT_IRRIGATION_TIME = 15;    // 15 seconds
constexpr int DEFAULT_PHOTOPERIOD = 22;        // VEG mode (main_old.cpp defined VEG as 22)
constexpr bool DEFAULT_AUTO_IRRIGATION = false;
constexpr bool DEFAULT_POWER_SAVING = false;

// Constants from main_old.cpp
constexpr int VEG = 22;  // vegetativo
constexpr int FLOR = 33; // floração
constexpr int GER = 11;  // germinação

// Initialize static instance
ConfigManager *ConfigManager::instance = nullptr;

ConfigManager::ConfigManager()
    : irrigationIntervalInDays(DEFAULT_IRRIGATION_INTERVAL),
      irrigationTimeInSeconds(DEFAULT_IRRIGATION_TIME),
      photoperiod(DEFAULT_PHOTOPERIOD),
      autoIrrigationEnabled(DEFAULT_AUTO_IRRIGATION),
      powerSavingEnabled(DEFAULT_POWER_SAVING)
{
}

ConfigManager &ConfigManager::getInstance()
{
    if (instance == nullptr)
    {
        instance = new ConfigManager();
    }
    return *instance;
}

void ConfigManager::begin()
{
    // Initialize EEPROM
    EEPROM.begin(EEPROM_SIZE);

    // Check if we have a valid configuration
    int version = readInt(VERSION_ADDRESS);
    if (version != CONFIG_VERSION)
    {
        // First time running or version changed, initialize defaults
        resetToDefaults();
    }

    // Load configuration
    load();
}

void ConfigManager::load()
{
    // Read values from EEPROM
    irrigationIntervalInDays = readInt(IRRIGATION_INTERVAL_ADDRESS);
    irrigationTimeInSeconds = readInt(IRRIGATION_TIME_ADDRESS);
    photoperiod = readInt(PHOTOPERIOD_ADDRESS);
    autoIrrigationEnabled = readBool(AUTO_IRRIGATION_ADDRESS);
    powerSavingEnabled = readBool(POWER_SAVING_ADDRESS);
}

void ConfigManager::save()
{
    // Write version first
    writeInt(VERSION_ADDRESS, CONFIG_VERSION);

    // Write all configuration values
    writeInt(IRRIGATION_INTERVAL_ADDRESS, irrigationIntervalInDays);
    writeInt(IRRIGATION_TIME_ADDRESS, irrigationTimeInSeconds);
    writeInt(PHOTOPERIOD_ADDRESS, photoperiod);
    writeBool(AUTO_IRRIGATION_ADDRESS, autoIrrigationEnabled);
    writeBool(POWER_SAVING_ADDRESS, powerSavingEnabled);

    // Commit changes to EEPROM
    EEPROM.commit();
}

void ConfigManager::resetToDefaults()
{
    // Reset to default values
    irrigationIntervalInDays = DEFAULT_IRRIGATION_INTERVAL;
    irrigationTimeInSeconds = DEFAULT_IRRIGATION_TIME;
    photoperiod = DEFAULT_PHOTOPERIOD;
    autoIrrigationEnabled = DEFAULT_AUTO_IRRIGATION;
    powerSavingEnabled = DEFAULT_POWER_SAVING;

    // Save defaults to EEPROM
    save();
}

int ConfigManager::getIrrigationInterval() const
{
    return irrigationIntervalInDays;
}

int ConfigManager::getIrrigationTime() const
{
    return irrigationTimeInSeconds;
}

int ConfigManager::getPhotoperiod() const
{
    return photoperiod;
}

bool ConfigManager::isAutoIrrigationEnabled() const
{
    return autoIrrigationEnabled;
}

bool ConfigManager::isPowerSavingEnabled() const
{
    return powerSavingEnabled;
}

void ConfigManager::setIrrigationInterval(int days)
{
    if (days > 0)
    {
        irrigationIntervalInDays = days;
        writeInt(IRRIGATION_INTERVAL_ADDRESS, days);
        EEPROM.commit();
    }
}

void ConfigManager::setIrrigationTime(int seconds)
{
    if (seconds > 0)
    {
        irrigationTimeInSeconds = seconds;
        writeInt(IRRIGATION_TIME_ADDRESS, seconds);
        EEPROM.commit();
    }
}

void ConfigManager::setPhotoperiod(int period)
{
    if (period == VEG || period == FLOR || period == GER)
    {
        photoperiod = period;
        writeInt(PHOTOPERIOD_ADDRESS, period);
        EEPROM.commit();
    }
}

void ConfigManager::setAutoIrrigationEnabled(bool enabled)
{
    autoIrrigationEnabled = enabled;
    writeBool(AUTO_IRRIGATION_ADDRESS, enabled);
    EEPROM.commit();
}

void ConfigManager::setPowerSavingEnabled(bool enabled)
{
    powerSavingEnabled = enabled;
    writeBool(POWER_SAVING_ADDRESS, enabled);
    EEPROM.commit();
}

void ConfigManager::writeInt(int address, int value)
{
    // Write int (4 bytes)
    EEPROM.put(address, value);
}

void ConfigManager::writeBool(int address, bool value)
{
    // Write bool (1 byte)
    EEPROM.write(address, value ? 1 : 0);
}

int ConfigManager::readInt(int address)
{
    int value;
    EEPROM.get(address, value);
    return value;
}

bool ConfigManager::readBool(int address)
{
    return EEPROM.read(address) != 0;
}