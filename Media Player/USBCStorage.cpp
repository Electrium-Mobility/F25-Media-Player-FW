#include "USBCStorage.h"

USBCStorage:USBCStorage()
    : connected(false), mounted(false), mountPoint("/usb") {

}

USBCD::~UBCStorage() {
    if(mounted) {
        unmountUSB();
    }
}

bool USBCStorage::detectConnection() {
    // TODO: finish
}

esp_err_t USBCStorage:mountUSB(const char* path) {
    // TODO: finish
}

esp_err_t USBCStorage::unmountUSB(){
    // TODO: finish
}

void USBCStorage::transferFiles() {
    // TODO: finish
}

std::vector<std::string> USBCStorage::scanFiles() {
    // TODO: finish
}