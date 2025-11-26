/**
 * AudioPipeline - C skeleton (ESP-IDF/ESP-ADF friendly)
 * Manages the audio processing pipeline and its elements.
 *
 * Note: This is a pure-C skeleton. External types from ESP-ADF are kept opaque (void* or int enums).
 */
#ifndef AUDIO_PIPELINE_WRAPPER_H
#define AUDIO_PIPELINE_WRAPPER_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

struct AudioPipeline {
    void * pipeline_handle; /* audio_pipeline_handle_t (opaque). */
    void ** elements; /* Array of audio_element_handle_t pointers (opaque). */
    int elements_count; /* Number of registered elements. */
    int state; /* audio_pipeline_state_t value. */
};

/* API */
void * audio_pipeline_wrapper_create(struct AudioPipeline *pl);
esp_err_t audio_pipeline_wrapper_register_element(struct AudioPipeline *pl, void *element);
esp_err_t audio_pipeline_wrapper_link_elements(struct AudioPipeline *pl);
esp_err_t audio_pipeline_wrapper_start(struct AudioPipeline *pl);
esp_err_t audio_pipeline_wrapper_pause(struct AudioPipeline *pl);
esp_err_t audio_pipeline_wrapper_stop(struct AudioPipeline *pl);
esp_err_t audio_pipeline_wrapper_resume(struct AudioPipeline *pl);
int audio_pipeline_wrapper_get_state(struct AudioPipeline *pl);

#ifdef __cplusplus
}
#endif

#endif /* AUDIO_PIPELINE_WRAPPER_H */
