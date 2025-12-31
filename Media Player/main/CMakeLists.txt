idf_component_register(SRCS "main.cpp"
                    REQUIRES driver SDCardManager MusicPlayer fatfs esp_timer spiffs
                    INCLUDE_DIRS ".")
spiffs_create_partition_image(spiffs ../spiffs_image FLASH_IN_PROJECT)