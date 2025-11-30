// NOTE
// Using CPP files require add on materials such as [extern "C"] or [__cplusplus]
// since ESP-IDF is C based
#include <stdio.h>
#include "esp_log.h"

#define TAG "main"
#define TESTING_MODE 1
#if TESTING_MODE
#include "SDCardManager.h"
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
    SDCardManager cardModule = SDCardManager{};
    printf("Mount Success: %b\n", cardModule.mountSD());
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
    ESP_LOGI(TAG, "Sorting fileList");
    cardModule.sortFilesByNameAscending();
    cardModule.showFileList();
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