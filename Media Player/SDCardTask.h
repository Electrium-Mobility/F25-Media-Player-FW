#include <string>
#include <vector>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"


class SDCardTask {
public:
    // Attributes (from UML)
    bool sdMountStatus;
    std::vector<std::string> fileList;
    QueueHandle_t queueHandle;

    // Functions (from UML)
    bool mountSD();
    std::vector<std::string> listFiles();
    void* openFile(const std::string& filePath);   // placeholder: FileHandle
    void closeFile(const std::string& filePath);
    std::vector<uint8_t> readFileChunk(void* fileHandle);
    void handleRequest();
};
