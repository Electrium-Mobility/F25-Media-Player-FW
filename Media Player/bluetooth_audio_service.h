/**
 * BluetoothAudioService - C skeleton (ESP-IDF/ESP-ADF friendly)
 * High-level wrapper around Bluetooth audio service APIs from ESP-ADF.
 *
 * Note: This is a pure-C skeleton. External types from ESP-ADF are kept opaque (void* or int enums).
 */
#ifndef BLUETOOTH_AUDIO_SERVICE_H
#define BLUETOOTH_AUDIO_SERVICE_H

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
    int mode; /* bluetooth_mode_t value (A2DP_SOURCE, etc.). */
    void * stream_handle; /* audio_element_handle_t pointer (opaque). */
    bool service_started; /* Whether the service is currently running. */
};

/* API */
void bluetooth_audio_service_configure(struct BluetoothAudioService *svc, const char *name, int mode);
esp_err_t bluetooth_audio_service_start(struct BluetoothAudioService *svc);
esp_err_t bluetooth_audio_service_stop(struct BluetoothAudioService *svc);
void * bluetooth_audio_service_create_stream(struct BluetoothAudioService *svc);
int bluetooth_audio_service_get_connection_status(struct BluetoothAudioService *svc);

#ifdef __cplusplus
}
#endif

#endif /* BLUETOOTH_AUDIO_SERVICE_H */
