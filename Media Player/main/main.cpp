// NOTE
// Using CPP files require add on materials such as [extern "C"] or [__cplusplus]
// since ESP-IDF is C based
#include <stdio.h>
#include "esp_log.h"
#include <sys/unistd.h>

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
    SDCardManager cardModule = SDCardManager(true);
    //printf("Mount Success: %b\n", cardModule.mountSD());
    ESP_LOGI(TAG, "Showing Empty fileLists");
    cardModule.showFileList();
    ESP_LOGI(TAG, "Manually Calling fileList Update");
    cardModule.updateFileListFromCard("/");
    ESP_LOGI(TAG, "Printing Updated list");
    cardModule.showFileList();
    ESP_LOGI(TAG, "Showing fileLists with explicit path");
    cardModule.showFileList("/");
    // cardModule.showFileList("My Folder");
    // cardModule.showFileList("Error");
    printf("Current File: %s", cardModule.getAbsCurrentFilePath().c_str());
    ESP_LOGI(TAG, "Sorting fileList");
    cardModule.sortFilesByNameAscending();
    cardModule.showFileList();
    MusicPlayer mp = MusicPlayer{};
    mp.testInitI2S();
    
    printf("cwd = %s\n", getcwd(NULL, 0));
    // If it prints "/", it means the POSIX layer does not recognize sdcard as a root
    cardModule.incrementCurrentFile();
    cardModule.incrementCurrentFile();
    mp.testWavAudioI2s(cardModule.getAbsCurrentFilePath().c_str());
    cardModule.decrementCurrentFile();
    mp.testWavAudioI2s(cardModule.getAbsCurrentFilePath().c_str());
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