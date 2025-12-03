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
    string currentFile;
    size_t currentFileIndex;

    
    SDCardManager(bool mount);
    bool sortFilesByName();
    bool sortFilesByNameAscending();
    bool shuffle();
    //bool sortFilesByType();
    FILE getFile(const char *path);
    FILINFO getFileInfo(const char *path);
    string getAbsCurrentFilePath();
    void incrementCurrentFile();
    void decrementCurrentFile();
    
    // Functions (from UML)
    bool mountSD();
    void updateFileListFromCard(const char *path);
    void showFileList(const char *path = nullptr);
    void* openFile(const string& filePath);   // placeholder: FileHandle
    void closeFile(const string& filePath);
    vector<uint8_t> readFileChunk(void* fileHandle);
    void handleRequest();
};
