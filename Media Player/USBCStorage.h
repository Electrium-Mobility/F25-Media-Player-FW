#ifndef USBC_STORAGE_H
#define USBC_STORAGE_H

#include "esp_err.h"
#include <string>
#include <vector>

class USBCStorage {
private:
    bool connected;
    bool mounted;
    std::string mountPoint;

public:
    USBCStorage();
    ~USBCStorage();

    bool detectConnection();

    esp_err_t mountUSB(const char* path);

    esp_err_t unmountUSB();

    void transferFiles();

    std::vector<std::string> scanFiles();

    bool isConnected() const {
        return connected;
    }

    bool isMounted() const {
        return mounted;
    }
};

#endif