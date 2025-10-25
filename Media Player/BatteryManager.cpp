#include "BatteryManager.h"

BatteryManager::BatteryManager()
    : currentLevel(100), charging(false), voltage(4,2f), adcChannel(BATTERY_ADC_CHANNEL), adcChars(nullptr) {

}

BatteryManager::~BatteryManager() {
    // TODO: complete
}

esp_err_t BatteryManager::initalize() {
    // TODO: complete
}

float BatteryManager::readVoltage() {
    // TODO: complete
}

int BatteryManager::voltageToPercentage(float v) {
    // TODO: complete
}

int BatteryManager::getBatteryLevel() {
    return currentLevel;
}

bool BatteryManager::isCharging() {
    return charging;
}

void BatteryManager::monitorBattery() {
    // TODO: complete
}

void BatteryManager::enableLowPowerMode() {
    // TODO: complete
}
