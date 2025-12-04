/**
 * AudioPipeline - C skeleton (ESP-IDF/ESP-ADF friendly)
 * Manages the audio processing pipeline and its elements.
 *
 * Note: This is a pure-C skeleton. External types from ESP-ADF are kept opaque (void* or int enums).
 */
#ifndef AudioPipelineWrapper_H
#define AudioPipelineWrapper_H

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
void * AudioPipelineWrapper_create(struct AudioPipeline *pl);
esp_err_t AudioPipelineWrapper_register_element(struct AudioPipeline *pl, void *element);
esp_err_t AudioPipelineWrapper_link_elements(struct AudioPipeline *pl);
esp_err_t AudioPipelineWrapper_start(struct AudioPipeline *pl);
esp_err_t AudioPipelineWrapper_pause(struct AudioPipeline *pl);
esp_err_t AudioPipelineWrapper_stop(struct AudioPipeline *pl);
esp_err_t AudioPipelineWrapper_resume(struct AudioPipeline *pl);
int AudioPipelineWrapper_get_state(struct AudioPipeline *pl);

#ifdef __cplusplus
}
#endif

#endif /* AudioPipelineWrapper_H */
