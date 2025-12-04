/**
 * BluetoothAudioService - C skeleton (ESP-IDF/ESP-ADF friendly)
 * High-level wrapper around Bluetooth audio service APIs from ESP-ADF.
 *
 * Note: This is a pure-C skeleton. External types from ESP-ADF are kept opaque (void* or int enums).
 */
#ifndef BluetoothAudioService_H
#define BluetoothAudioService_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct BluetoothAudioService {
    void * cfg; /* bluetooth_service_cfg_t pointer (opaque). */
    char device_name[32]; /* Bluetooth device name. */
    int mode; /* bluetooth_mode_t value (A2dpSource, etc.). */
    void * stream_handle; /* audio_element_handle_t pointer (opaque). */
    bool service_started; /* Whether the service is currently running. */
};

/* API */
void BluetoothAudioService_configure(struct BluetoothAudioService *svc, const char *name, int mode);
esp_err_t BluetoothAudioService_start(struct BluetoothAudioService *svc);
esp_err_t BluetoothAudioService_stop(struct BluetoothAudioService *svc);
void * BluetoothAudioService_create_stream(struct BluetoothAudioService *svc);
int BluetoothAudioService_get_connection_status(struct BluetoothAudioService *svc);

#ifdef __cplusplus
}
#endif

#endif /* BluetoothAudioService_H */
