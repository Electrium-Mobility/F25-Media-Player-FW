// NOTE
// Using CPP files require add on materials such as [extern "C"] or [__cplusplus]
// since ESP-IDF is C based
#include <stdio.h>
#include "esp_log.h"
#include <sys/unistd.h>
//extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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

void playMP3Thread(void * pvParameters ) {
    MusicPlayer mp = MusicPlayer{};
    mp.testInitI2S();
    while (true) mp.testMP3AudioI2S("/sdcard/Full Moon Full Life - Persona 3 Reload Original Soundtrack.mp3");
    
}

void alternateTask(void) {
    esp_err_t init = esp_timer_early_init();

    SDCardManager cardModule = SDCardManager(false);
    if (!cardModule.mountSD()) {
        return;
    }
    //ESP_LOGI(TAG, "Showing Empty fileLists");
    //cardModule.showFileList();
    //ESP_LOGI(TAG, "Manually Calling fileList Update");
    cardModule.updateFileListFromCard("/");
    //ESP_LOGI(TAG, "Printing Updated list");
    //cardModule.showFileList();
    //ESP_LOGI(TAG, "Showing fileLists with explicit path");
    //cardModule.showFileList("/");
    // cardModule.showFileList("My Folder");
    // cardModule.showFileList("Error");
    printf("Current File: %s", cardModule.getAbsCurrentFilePath().c_str());
    MusicPlayer mp = MusicPlayer{};
    mp.testInitI2S();
    // while(cardModule.currentFileIndex < cardModule.fileList.size()-1) {
    //     mp.testMP3AudioI2S(cardModule.getAbsCurrentFilePath().c_str());
    //     cardModule.incrementCurrentFile();
    // }

    ESP_LOGI(TAG, "Sorting fileList");
    cardModule.sortFilesByNameAscending();
    cardModule.showFileList();
    
    
    printf("cwd = %s\n", getcwd(NULL, 0));
    // If it prints "/", it means the POSIX layer does not recognize sdcard as a root
    cardModule.incrementCurrentFile();
    //cardModule.incrementCurrentFile();
    //mp.testWavAudioI2S(cardModule.getAbsCurrentFilePath().c_str());
    //cardModule.zeroCurrentFile();
    // while(cardModule.currentFileIndex < cardModule.fileList.size()-1) {
    //     printf("Free mem: %d\n", heap_caps_get_free_size(8));
    //     mp.testMP3AudioI2S(cardModule.getAbsCurrentFilePath().c_str());
    //     cardModule.incrementCurrentFile();
    //     //vTaskDelay(pdMS_TO_TICKS(100)); 
    //     // FOR SOME REASON FREERTOS.H DOES NOT HAVE vTaskDelay ENABLED
    //     // IDK HOW TO TURN IT ON IN MENUCONFIG
    //     // There a problem with the freeRTOS import actually, it's not the function itself.
    //     // Probably has to do with this project being c++ but everything else is c
    // }

    //mp.testCloseI2S();



    for (;SONG > -1; SONG--) {
        //SONG = 6;
        printf("Free mem: %d\n", heap_caps_get_free_size(8));
        mp.volume = 0.25;
        if (SONG == 0) {
            mp.testMP3AudioI2S("/sdcard/440Hz.mp3");
            mp.testWavAudioI2S("/sdcard/440Hz.wav");     
        } else if (SONG == 1) {
            mp.testMP3AudioI2S("/sdcard/Breath of the Wild (Main Theme) - The Legend of Zelda Breath of the Wild Soundtrack.mp3");
            mp.testWavAudioI2S("/sdcard/Breath of the Wild (Main Theme) - The Legend of Zelda Breath of the Wild Soundtrack.wav");
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
        } else if (SONG == 5) {
            //mp.testMP3AudioI2S("/sdcard/Luna Haruna - Overfly.mp3");
            //mp.testWavAudioI2S("/sdcard/Luna Haruna - Overfly.wav");   
        } else {
            mp.testMP3AudioI2S("/sdcard/Full Moon Full Life - Persona 3 Reload Original Soundtrack.mp3");
            mp.testWavAudioI2S("/sdcard/Full Moon Full Life - Persona 3 Reload Original Soundtrack.wav");
        }
    }
    // TaskHandle_t xHandle = NULL;
    // xTaskCreate(playMP3Thread, "Play Mp3", 4096, NULL, 1, &xHandle);
    // configASSERT( xHandle );

    //mp.testCloseI2S();

    // You need to use absolute paths here
    
    //cardModule.sortFilesByName();
    //cardModule.showFileList();
}

int app_main(void)
{
    printf("Hello World!\n");
    if (TESTING_MODE) {
        alternateTask();
    }
    return 0;
}