#include <string>
#include <vector>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "sdmmc_cmd.h"
#include "ff.h"
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

class SDCardManager {
public:
    // Attributes (from UML)
    bool sdMountStatus;
    vector<string> fileList;
    QueueHandle_t queueHandle; // Not sure what the queue is supposed to do
    sdmmc_card_t *card;
    // TODO: You need a current status field, maybe FILE type but how to deal with MP3
    // You also need sorted list of files in current DIR... 

    
    
    bool sortFilesByName();
    bool sortFilesByDate();
    FILE getFile(const char *path);
    FILINFO getFileInfo(const char *path);
    // Some kind of getFile functionality look into FILE features
    // Shuffle?
    // getCurrentStatus
    
    // Functions (from UML)
    bool mountSD();
    void listFilesFromCard(const char *path);
    void showFileList(const char *path);
    void* openFile(const string& filePath);   // placeholder: FileHandle
    void closeFile(const string& filePath);
    vector<uint8_t> readFileChunk(void* fileHandle);
    void handleRequest();
};
