#ifndef BATTERY_MANAGER_H
#define BATTERY_MANAGER_H

#include "esp_err.h"

class BatteryManager {
private:
    int currentLevel;
    bool charging;
    float voltage;
    adc1_channel_t adcChannel;
    esp_adc_cal_characteristics_t* adcChars;

    int voltageToPercentage(float voltage)

public:
    BatteryManager();
    ~BatteryManager();

    // Initalizing battery monitoring
    esp_err_t initalize();

    // Get battery level
    int getBatteryLevel();

    // Read raw battery voltage
    float readVoltage();

    // Check if charging
    bool isCharging();

    // Monitor battery continuously (call periodically)
    void monitorBattery();

    // Enable low power mode
    void enableLowPowerMode();
};

#endif