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

void alternateTask(void) {
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
    cardModule.showFileList("/");
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
    //while(cardModule.currentFileIndex < cardModule.fileList.size()-1) {
        mp.testMP3AudioI2S(cardModule.getAbsCurrentFilePath().c_str());
        //cardModule.incrementCurrentFile();
        //vTaskDelay(pdMS_TO_TICKS(100)); 
        // FOR SOME REASON FREERTOS.H DOES NOT HAVE vTaskDelay ENABLED
        // IDK HOW TO TURN IT ON IN MENUCONFIG
        // There a problem with the freeRTOS import actually, it's not the function itself.
        // Probably has to do with this project being c++ but everything else is c
    //}

    //mp.testCloseI2S();
    
    //mp.testWavAudioI2S("/sdcard/Its Going Down Now - Persona 3 Reload Original Soundtrack.wav");
    mp.testCloseI2S();

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