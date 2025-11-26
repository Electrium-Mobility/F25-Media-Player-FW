/**
 * BluetoothController - C skeleton (ESP-IDF/ESP-ADF friendly)
 * Controls the low-level Bluetooth radio hardware on the ESP32.
 *
 * Note: This is a pure-C skeleton. External types from ESP-ADF are kept opaque (void* or int enums).
 */
#ifndef BLUETOOTH_CONTROLLER_H
#define BLUETOOTH_CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

struct BluetoothController {
    bool controller_initialized; /* Whether controller hardware is initialized. */
    bool controller_enabled; /* Whether controller is currently enabled. */
    void * controller_config; /* esp_bt_controller_config_t pointer (opaque in this skeleton). */
};

/* API */
esp_err_t bluetooth_controller_init(struct BluetoothController *bc, void *cfg);
esp_err_t bluetooth_controller_enable(struct BluetoothController *bc, int mode);
esp_err_t bluetooth_controller_disable(struct BluetoothController *bc);
int bluetooth_controller_get_status(struct BluetoothController *bc);

#ifdef __cplusplus
}
#endif

#endif /* BLUETOOTH_CONTROLLER_H */
