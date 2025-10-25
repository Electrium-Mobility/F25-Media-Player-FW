#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "esp_err.h"
#include <string>

enum class Scree {
    TRACK_INFO,
    MENU,
    SETTINGS,
    PLAYLIST
};

class LCDDisplay {
private:
    int brightness;
    Screen currentScreen;
    i2c_port_t i2cPort;

    // Send command to LCD
    esp_err_t sendCommand(uint8_t cmd);

    // Send data to LCD
    esp_err_t sendData(uint8_t data);

public:

    LCDDisplay();
    ~LCDDisplay();

    // Initalize LCD
    esp_err_t initialize();

    // Display track information
    void displayTrackInfo(const std::string& track, const std::string& artist);

    // Display battery status
    void displayBatteryStatus(int level);

    // Display volume lvl
    void displayVolume(int level);

    void displayBluetoothStatus(bool connected, const std::string& deviceName);

    void showMenu();

    void setBrightness(int level);

    void clearScreen();

    void setScreen(Screen screen) {
        currentScreen = screen;
    }
};

#endif