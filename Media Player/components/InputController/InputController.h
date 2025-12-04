/**
 * InputController - C skeleton (ESP-IDF/ESP-ADF friendly)
 * Handles physical button inputs via GPIO.
 *
 * Note: This is a pure-C skeleton. External types from ESP-ADF are kept opaque (void* or int enums).
 */
#ifndef InputController_H
#define InputController_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

class InputController {
    int * gpio_buttons; /* Array of GPIO numbers used for buttons. */
    int gpio_buttons_count; /* Number of GPIOs. */
    uint32_t last_button_press_ms; /* Last press timestamp for debouncing. */
public:
        /* API */
    esp_err_t InputController_init_buttons(struct InputController *ic, const int *gpios, int count);
    void InputController_handle_play_pause(void);
    void InputController_handle_next(void);
    void InputController_handle_previous(void);
    void InputController_handle_volume_up(void);
    void InputController_handle_volume_down(void);
    void InputController_handle_menu(void);
};



#ifdef __cplusplus
}
#endif

#endif /* InputController_H */
