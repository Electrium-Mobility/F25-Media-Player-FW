#pragma once
#include "esp_err.h"


struct esp_a2d_handle_t {};   // Replace with actual type from ESP-IDF
using QueueHandle_t = void*;  // Replace with <freertos/queue.h> later


class BluetoothServiceTask {
public:
    // Attributes
    esp_a2d_handle_t btHandle;
    QueueHandle_t btStatusQueue;

    // Functions
    esp_err_t initBluetooth();
    esp_err_t connectSinkDevice();
    esp_err_t receiveAudioStream();
    void      processControlSignals();
    void      stopBluetooth();
    void      vTaskDelay(int ms);
};
