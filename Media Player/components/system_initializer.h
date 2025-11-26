/**
 * SystemInitializer - C skeleton (ESP-IDF/ESP-ADF friendly)
 * Handles early ESP32 setup such as NVS and logging level.
 *
 * Note: This is a pure-C skeleton. External types from ESP-ADF are kept opaque (void* or int enums).
 */
#ifndef SYSTEM_INITIALIZER_H
#define SYSTEM_INITIALIZER_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>
#include <esp_log.h>
#include <nvs_flash.h>

#ifdef __cplusplus
extern "C" {
#endif

struct SystemInitializer {
    bool nvs_initialized; /* Flag tracking whether Non-Volatile Storage (NVS) is ready. */
    int log_level; /* Current logging verbosity level (uses esp_log_level_t values). */
};

/* API */
esp_err_t system_initializer_init_nvs(struct SystemInitializer *si);
void system_initializer_set_log_level(struct SystemInitializer *si, int level);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_INITIALIZER_H */
