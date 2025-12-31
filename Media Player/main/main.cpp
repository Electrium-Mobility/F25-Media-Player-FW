// NOTE
// Using CPP files require add on materials such as [extern "C"] or [__cplusplus]
// since ESP-IDF is C based
#include <stdio.h>
#include "esp_log.h"
#include <sys/unistd.h>
#include <driver/i2c_master.h>
//extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiffs.h"
#include <stdint.h>
#include <stdlib.h>
// You need to import FreeRTOS before task
//}


#define TAG "main"
#define TESTING_MODE 1


#if TESTING_MODE
#include "SDCardManager.h"
#include "MusicPlayer.h"
#include "esp_timer.h"
#endif
// --------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

int app_main();

#ifdef __cplusplus
}
#endif
// --------------------------------------------------------------------------
int SONG = 6;

void testSDREAD(void * pvParameters ) {
    SDCardManager cardModule = SDCardManager(false);
    if (!cardModule.mountSD()) {
        return;
    }
    cardModule.testConstantRead();
    
}

void testSpiffWav(void) {
    
    
    TaskHandle_t xHandle = NULL;
    xTaskCreate(testSDREAD, "Play Mp3", 10000, NULL, 1, &xHandle);
    configASSERT( xHandle );

    

    esp_vfs_spiffs_conf_t config = {
        .base_path="/spiffs",
        .partition_label= NULL,
        .max_files=5,
        .format_if_mount_failed=true
    };

    //Upload the files under the folder 'partition' on this computer
    esp_err_t result = esp_vfs_spiffs_register(&config);

    //Error logging
    if (result!=ESP_OK){
        ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(result));
        return;
    }

    size_t total=0, used=0;
    //Retrieves information on memory usage
    result = esp_spiffs_info(config.partition_label, &total, &used);

    if (result!=ESP_OK){
       ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(result)); 
    } else{
        ESP_LOGI(TAG, "Free Memory: (%d) Memory Used: (%d)", total, used);
    }

    MusicPlayer mp = MusicPlayer{};
    mp.testInitI2S();
    mp.testRunI2C();
    //mp.testWavAudioI2S("/spiffs/Silence.wav");
    mp.testMP3AudioI2S("/spiffs/Silence.mp3");
    mp.testCloseI2C();
    mp.testCloseI2S();
}

void testCompareWavMP3(void) {

    
    // ----------------------------------------------------------
    SDCardManager cardModule = SDCardManager(false);
    if (!cardModule.mountSD()) {
        return;
    }
    MusicPlayer mp = MusicPlayer{};
    mp.testInitI2S();
    mp.testRunI2C();
    for (;SONG > -1; SONG--) {
        //SONG = 6;
        printf("Free mem: %d\n", heap_caps_get_free_size(8));
        //mp.volume = 0.25;
        if (SONG == 5) {
            mp.testMP3AudioI2S("/sdcard/440Hz.mp3");
            mp.testWavAudioI2S("/sdcard/440Hz.wav");     
        } else if (SONG == 6) {
            //break;
            //mp.testMP3AudioI2S("/sdcard/Breath of the Wild (Main Theme) - The Legend of Zelda Breath of the Wild Soundtrack.mp3");
            //ESP_LOGI(TAG, "Waiting 5s");
            //vTaskDelay(pdMS_TO_TICKS(5000));
            //mp.testWavAudioI2S("/sdcard/Breath of the Wild (Main Theme) - The Legend of Zelda Breath of the Wild Soundtrack.wav");
            break;
        } else if (SONG == 2) {
            mp.volume = 0.1;
            mp.testMP3AudioI2S("/sdcard/It's Going Down Now - Persona 3 Reload Original Soundtrack.mp3");
            mp.testWavAudioI2S("/sdcard/It's Going Down Now - Persona 3 Reload Original Soundtrack.wav");
        } else if (SONG == 3) {
            mp.testMP3AudioI2S("/sdcard/Pokemon XY Anime OST_ BW Title Screen(XY Ver).mp3");
            mp.testWavAudioI2S("/sdcard/Pokemon XY Anime OST_ BW Title Screen(XY Ver).wav");
        } else if (SONG == 4) {
            mp.testMP3AudioI2S("/sdcard/Black Tar - Xenoblade Chronicles X.mp3");
            mp.testWavAudioI2S("/sdcard/Black Tar - Xenoblade Chronicles X.wav");
        } else if (SONG == 1) {
            mp.testMP3AudioI2S("/sdcard/Luna Haruna - Overfly.mp3");
            mp.testWavAudioI2S("/sdcard/Luna Haruna - Overfly.wav");   
        } else {
            mp.testMP3AudioI2S("/sdcard/Full Moon Full Life - Persona 3 Reload Original Soundtrack.mp3");
            mp.testWavAudioI2S("/sdcard/Full Moon Full Life - Persona 3 Reload Original Soundtrack.wav");
        }
    }

    mp.testCloseI2S();
    mp.testCloseI2C();
}

void testBasicPlayback(void) {
    vTaskDelay(pdMS_TO_TICKS(1000));
    esp_err_t init = esp_timer_early_init();
    SDCardManager cardModule = SDCardManager(false);
    if (!cardModule.mountSD()) {
        return;
    }
    cardModule.updateFileListFromCard("/");
    cardModule.sortFilesByNameAscending();
    cardModule.showFileList();
    printf("Current File: %s\n", cardModule.getAbsCurrentFilePath().c_str());
    MusicPlayer mp = MusicPlayer{};
    mp.testInitI2S();
    ESP_LOGI(TAG, "Setting I2C...");
    mp.testRunI2C();

    //mp.testMP3AudioI2S("/sdcard/Black Tar - Xenoblade Chronicles X.mp3");
    //mp.testWavAudioI2S("/sdcard/Black Tar - Xenoblade Chronicles X.wav");
    cardModule.incrementCurrentFile();
    cardModule.incrementCurrentFile();
    while(cardModule.currentFileIndex < cardModule.fileList.size()) {
        // You need to use absolute paths here
        mp.testMP3AudioI2S(cardModule.getAbsCurrentFilePath().c_str());
        cardModule.incrementCurrentFile();
    }
    
    
    // printf("cwd = %s\n", getcwd(NULL, 0));
    // If it prints "/", it means the POSIX layer does not recognize sdcard as a root
    cardModule.incrementCurrentFile();  

    // TaskHandle_t xHandle = NULL;
    // xTaskCreate(playMP3Thread, "Play Mp3", 4096, NULL, 1, &xHandle);
    // configASSERT( xHandle );

    mp.testCloseI2S();
}

int app_main(void)
{
    printf("--------------------------------------------------------------\n");
    if (TESTING_MODE) {
        //testBasicPlayback();
        //testCompareWavMP3();
        testSpiffWav();
    }
    return 0;
}