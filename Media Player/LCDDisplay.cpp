#include "LCDDisplay.h"
#include "esp_log.h"

LCDDisplay::LCDDisplay()
    : brightness(80), currentScreen(Screen::TRACK_INFO), i2cPort(LCD_I2C_PORT) {

    }

LCDDisplay::~LCDDisplay() {
    i2c_driver_delete(i2cPort);
}

esp_err_t LCDDisplay::intialize() {
    // TODO: finish
}

esp_err_t LCDDisplay::sendCommand(uint8_t cmd) {
    // TODO: finish
}

esp_err_t LCDDisplay::sendData(uint8_t data) {
    // TODO: finish
}

void LCDDisplay::displayTrackInfo(const std::string& track, const std::string& artist) {
    // TODO: finish
}

void LCDDisplay::displayBatteryStatus(int level) {
    // TODO: finish   
}

void LCDDisplay::displayVolume(int level) {
    // TODO: finish
}

void LCDDisplay::displayBluetoothStatus(bool connected, const std::string& deviceName) {
    // TODO: finish
}

void LCDDisplay::showMenu() {
    // TODO: finish
}

void LCDDisplay::setBrightness(int level) {
    // TODO: finish
}

void LCDDisplay::clearScreen() {
    // TODO: finish
}