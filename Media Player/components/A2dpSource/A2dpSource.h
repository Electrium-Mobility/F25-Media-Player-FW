/**
 * A2dpSource - C skeleton (ESP-IDF/ESP-ADF friendly)
 * Implements A2DP as a source device (sends audio to sinks).
 *
 * Note: This is a pure-C skeleton. External types from ESP-ADF are kept opaque (void* or int enums).
 */
#ifndef A2dpSource_H
#define A2dpSource_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

struct A2dpSource {
    bool initialized; /* Whether A2DP source is initialized. */
    bool connected; /* Whether connected to an A2DP sink. */
    uint8_t peer_address[6]; /* Bluetooth MAC address of connected device. */
    int connection_state; /* esp_a2d_connection_state_t value. */
    int audio_state; /* esp_a2d_audio_state_t value. */
};

/* API */
esp_err_t A2dpSource_register_callback(void (*cb)(int event, void *param));
esp_err_t A2dpSource_register_data_callback(int (*cb)(const uint8_t *data, uint32_t len));
esp_err_t A2dpSource_init(struct A2dpSource *a2);
esp_err_t A2dpSource_connect(struct A2dpSource *a2, const uint8_t peer_bda[6]);
esp_err_t A2dpSource_disconnect(struct A2dpSource *a2);
void A2dpSource_event_handler(int event, void *param);
int32_t A2dpSource_data_handler(const uint8_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* A2dpSource_H */
