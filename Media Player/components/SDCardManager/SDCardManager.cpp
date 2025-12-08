#include "SDCardManager.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/errno.h>

#define MOUNT_POINT "/sdcard"
#define TAG "SDCardManager"
#define MAX_STR_LEN 276
#define ASCII_LOWCASE_L 97 // a
#define ASCII_LOWCASE_H 122 // z
#define ASCII_UPCASE_L 65 // A
#define ASCII_UPCASE_H 90 // Z

using SD = SDCardManager;

SD::SDCardManager(bool mount) :
    sdMountStatus(false),
    fileList(),
    queueHandle(nullptr),
    card(nullptr),
    currentFile(),
    currentFileIndex(0)
    {
    
    if (mount) {
        mountSD();
    }
}

/**
 * @brief Mounts the SD Card Volume as VFS. Sets up required sdmmc interface.
 * @todo requires better error handling and probing. When board is brought up, make sure to use card detect
 * @return a boolean indicating whether the VFS FatFs creation was succesful
 */
bool SD::mountSD() {
    sdMountStatus = false;
    esp_err_t ret;
    esp_vfs_fat_mount_config_t mountConfig = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 512,
        .disk_status_check_enable = false,
        .use_one_fat = true,
    };
    sdmmc_slot_config_t slotConfig = SDMMC_SLOT_CONFIG_DEFAULT();
    // DARN, so compiling code in C++ vs in C will change certain things.
    // Before, you were allowed to use direct number conversions in the default configs
    // So for example the line ".d0 = 1," is valid as C will automatically convert that into
    // a gpio_num_t type. In C++, it will complain because 1 is an integer. Note that it will also
    // complain about other pins that you won't be using either because it's picky like that!
    // 
    slotConfig.width = 1;
    // Make sure you define this because in default config it is set to 0????
    ESP_LOGI(TAG, "Set SD bus width to 1");
    
    sdmmc_host_t hostConfig = SDMMC_HOST_DEFAULT();
    // TODO: might need to check out settings for this such as freq or speed?
    
    const char parentPath[] = MOUNT_POINT; //Remember, an array variable is a POINTER to the first element
    ret = esp_vfs_fat_sdmmc_mount( 
        parentPath, // Passing a refernce to an array means double pointer
        &hostConfig, 
        &slotConfig, 
        &mountConfig, 
        &(card) // function will define card
    );

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Mount failed: %d", ret);
        return false;
    }

    ESP_LOGI(TAG, "Mount Success: %d", ret);
    sdmmc_card_print_info(stdout, card); // This is why structs suck and classes are bettery
    // Anyways, seems like you can treat stdout as a FILE type?    
    sdMountStatus = true;
    return true;
}


/**
 * @brief Decodes a bitwise date from FILINFO. Returns the value YYYYMMDD to the OUT parameter
 * @param date a 16 bit WORD coming from a FILINFO data type 
 * @param out a c-style string of where the decoded value should be overwritten to. Should be at least 16 bytes
 */
void decodeDate(WORD date, char *out) {
    WORD d = date; 
    snprintf(out, 16, "%04d-%02d-%02d",
        (int) (((d >> 9) & 0x7F) + 1980),   // Year
        (int) ((d >> 5) & 0x0F),            // Month
        (int) (d & 0x1F)                    // Day
    );
}

/**
 * @brief Updates fileList with all present files listed in the path directory in the order
 * presented by the SD Card file system. This is considered unsorted
 * @param path a c-style string of the directory to be read. Pass in "/" for current directory,
 * or pass in "/My directory" or "My directory". The "/" is unnecessary.
 */
void SD::updateFileListFromCard(const char *path) {
    fileList.clear(); // Remove information from previous read

    // General procedure: open dir --> access/read dir --> close dir 
    // fopen() is from stdio.h, f_open() is from ff.h (FatFs module)
    // ff.h is automatically (implitly) included, my guess
    
    FF_DIR myDirectory;
    FRESULT res;
    // Check out the all possible FRESULT return values (integers) and their meaning!
    
    res = f_opendir(&myDirectory, path); // I thought you would pass "/sdcard", the MOUNT_POINT as the directory
    // But seems like FatFs Module does not recognize "/sdcard" as a true directory, but "/" is?
    // The alternative explanation is that the FatFs module is a high level library that simply just uses the directory
    // already set as the root directory (in this case esp_vfs_fats set it as "/sdcard"). It's just treating the sd card 
    // dir as base so it's trying to search for another "/sdcard" folder inside the real "/sdcard" dir.
    // Note that the POSIX layer does not know /sdcard is root!
    ESP_LOGI(TAG, "Opening Directory ErrNo: %d", res);
    if (res != 0) {
        ESP_LOGE(TAG, "Failed to open path. Path probably doesn't exist");
    }

    FILINFO myFile;
    char filePrintInfo[MAX_STR_LEN]; 
    char date[16];

    while ((res = f_readdir(&myDirectory, &myFile)) == 0 && myFile.fname[0] != '\0') {
        decodeDate(myFile.fdate, date);// The fdate field in FILINFO is encoded as not human readable
        // bits 0-4 are the days, bits 5-8 are the months, bits 9-15 are the years 
        
        // WE HAVE AN ISSUE
        // 1. filePrintInfo is previously 256 sized, but the LFN is also 256, so you'll have to forcibly truncate the file name as you write extra char
        //    Additionally, the snprintf function requires 255 (previously) but your LFN name is 256, is compile error
        // 2. Imagine that we have 300 songs in a single directory. Each song is 256 char, so = 76800 bytes
        //    Based off of the memory mapping produced every build having aroud 300k-ish bytes left, we should be careful!
        // 3. Certain songs make have Unicode characters in their name instead of pure ASCII, meaning LFN cannot register these names
        //    and will default to using SFN. This depends on where you get your music!
        snprintf(filePrintInfo, MAX_STR_LEN, "%s%s",  
            //date,
            (myFile.fattrib & AM_DIR) ? "** " : "",
            myFile.fname
        );
        //printf("%s\n", filePrintInfo); 
        
        std::string s = filePrintInfo;
        fileList.push_back(s);
        std::string d = date;
        fileDate.push_back(d);

        // Why is it not printing the full name?
        // Because Long File Name is not enabled. Currently falls back to Short File Name
        // To enable, go to menuconfig and set LFN (heap and stack options)
        // You will notice that there's a hidden directory called System Volume Information that Windows PC cannot detect
        // It's hidden by OS but manages storage handling. Direct POSIX operations can still detect it. Just leave alone.
    }
    if (res != 0) {
        ESP_LOGI(TAG, "Something went unexpected reading a file name! FRESULT: %d", res);
    }
    res = f_closedir(&myDirectory);
    ESP_LOGI(TAG, "Closing Directory ErrNo: %d", res);
    
    currentFileIndex = 0;
    currentFile = fileList[currentFileIndex];
    ESP_LOGI(TAG, "currentFileIndex: %d, currentFile: %s", currentFileIndex, currentFile);
}

/**
 * @brief Prints all files as is currently in the listed directory
 */
void SD::showFileList(const char *path) {
    ESP_LOGI(TAG, "Printing fileList");
    if (path != nullptr) {
        ESP_LOGI(TAG, "Calling fileList Update");
        updateFileListFromCard(path);
    }
    
    for (int i = 0; i < fileList.size(); i++) {
        printf("%s %s\n", fileDate[i].c_str(), fileList[i].c_str());
    }
    printf("# Files: %d\nTotal Capacity: %d\n", fileList.size(), fileList.capacity());

}



/**
 * @brief Performs insertion sort to order all files in ascending order by Name.
 * @return a boolean indicating whether a sort was sucessful or not
 * @deprecated
 * @note Using operators like ">" on Long File Name Strings might not work as intended. 
 * This is because LFN contains UTF-16 bytes instead of UTF-8/ASCII. This results in JUNK
 * decoding like H��?J < ���?↓, and thus failure to compare correctly.
 */
bool SD::sortFilesByName() {
    if (fileList.size() == 0) {
        ESP_LOGI(TAG, "Tried to sort file list when there are no files");
        return false;
    }
    for (int upper = 1; upper < fileList.size(); upper++) {
        std::string toBeChecked = fileList[upper];
        std::string dateAttribute = fileDate[upper];
        int lastIndex = upper;
        for (int i = upper; i > 0; i--) {
            if (fileList[upper] < fileList[i-1]) {
                ESP_LOGI(TAG, "%s < %s", fileList[upper], fileList[i-1]);
                fileList[i] = fileList[i-1];
                fileDate[i] = fileDate[i-1];
                lastIndex = i-1;
            }            
        }
        fileList[lastIndex] = toBeChecked;
        fileDate[lastIndex] = dateAttribute;
    }
    currentFileIndex = 0;
    currentFile = fileList[currentFileIndex];
    ESP_LOGI(TAG, "currentFileIndex: %d, currentFile: %s", currentFileIndex, currentFile);
    
    return true; 
}

/**
 * @brief Compare two characters, ignoring case if a letter in the alphabet. 
 * @param c1 a char from a string
 * @param c2 a char from a string
 * @return a signed char representing (c1 - c2). When =0, c1 == c2. When >0, c1 > c2. When <0, c1 < c2.
 * If c1 or c2 are both part of the alphabet but are different cases, c2 will be lowercased/capitalized 
 * to match c1, then (c1 - c2) is returned. Additionally, if c1 = '\0', it means str1 is shorter than str2,
 * so the function will return -1 < 0; The opposite is true when c2 = '\0'. This scenario should only be
 * reached if both strings happen to share the same characters all througout.
 */
signed char compareAlphabetIgnoreCase(char c1, char c2) {
    if (c1 == '\0') {
        return -1;
    }
    if (c2 == '\0') {
        return 1;
    }
    if (c1 == c2) {
        return 0;
    }
    if (ASCII_LOWCASE_L <= c1 && c1 <= ASCII_LOWCASE_H) {
        if (ASCII_UPCASE_L <= c2 && c2 <= ASCII_UPCASE_H) {
            return c1 - (c2 + 32);
        } else {
            return c1 - c2;
        }
    } else if (ASCII_UPCASE_L <= c1 && c1 <= ASCII_UPCASE_H) {
        if (ASCII_LOWCASE_L <= c2 && c2 <= ASCII_LOWCASE_H) {
            return c1 - (c2 - 32);
        } else {
            return c1 - c2;
        }
    } else {
        return c1 - c2;
    }
    
}

/**
 * @brief Sorts fileList alphabetically in ascending order based on name. This sort ignores letter cases, so
 * lower and upper cases aren't differentiated. 
 */
bool SD::sortFilesByNameAscending() {
    // ESP_LOGI(TAG, "%d A == A TRUE", compareAlphabetIgnoreCase('A', 'A'));
    // ESP_LOGI(TAG, "%d A == a TRUE", compareAlphabetIgnoreCase('A', 'a'));
    // ESP_LOGI(TAG, "%d a == A TRUE", compareAlphabetIgnoreCase('a', 'A'));
    // ESP_LOGI(TAG, "%d a == b FALSE", compareAlphabetIgnoreCase('a', 'b'));
    // ESP_LOGI(TAG, "%d a == B FALSE", compareAlphabetIgnoreCase('a', 'B'));
    // ESP_LOGI(TAG, "%d a == . FALSE", compareAlphabetIgnoreCase('a', '.'));

    if (fileList.size() == 0) {
        ESP_LOGI(TAG, "Tried to sort file list when there are no files");
        return false;
    }

    // Outer loop of insertion sort, determining who is next to be compared
    for (int upper = 1; upper < fileList.size(); upper++) {
        std::string toBeChecked = fileList[upper];
        std::string dateAttribute = fileDate[upper];
        int lastIndex = upper;
        //ESP_LOGI(TAG, "Comparing if %s", fileList[upper]);

        // Inner loop of insertion sort, determining where to [upper] should go
        for (int i = upper; i > 0; i--) {
            //ESP_LOGI(TAG, "UPPER VALUE %d", upper);
            char c1, c2;
            signed char compareResult = 0;
            // Loop through each string, comparing their characters at each index until they are not equal
            for (int j = 0; j < MAX_STR_LEN && compareResult == 0; j++) {
                //ESP_LOGI(TAG, "J VALUE %d", j);
                c1 = toBeChecked[j], c2 = fileList[i-1][j];  
                //ESP_LOGI(TAG, "%c < %c, COMPARING", c1, c2);       
                compareResult = compareAlphabetIgnoreCase(c1, c2);
                
            }
            //c1 = toBeChecked[0], c2 = fileList[i-1][0];
            //swap = compareAlphabetIgnoreCase(c1, c2) < 0;
            if (compareResult < 0) {
                //ESP_LOGI(TAG, "%c < %c, MOVED", c1, c2);
                fileList[i] = fileList[i-1];
                fileDate[i] = fileDate[i-1];
                lastIndex = i-1;
            }            
        }
        fileList[lastIndex] = toBeChecked;
        fileDate[lastIndex] = dateAttribute;
    }
    return true; 
}

std::string SD::getAbsCurrentFilePath() {
    return (std::string(MOUNT_POINT) + std::string("/") + currentFile);
}

/**
 * @todo PLEASE TEST THIS
 */
void SD::incrementCurrentFile() {
    if (currentFileIndex < fileList.size()-1) {
        currentFileIndex++;
    }
    currentFile = fileList[currentFileIndex];
    //ESP_LOGI(TAG, "currentFileIndex: %d, currentFile: %s", currentFileIndex, currentFile);
}

/**
 * @todo PLEASE TEST THIS
 */
void SD::decrementCurrentFile() {
    if (currentFileIndex > 0) {
        currentFileIndex--;
    }
    currentFile = fileList[currentFileIndex];
    ESP_LOGI(TAG, "currentFileIndex: %d, currentFile: %s", currentFileIndex, currentFile);
}

void SD::zeroCurrentFile() {
    currentFileIndex = 0;
    currentFile = fileList[currentFileIndex];
}


void* SD::openFile(const std::string& filePath) {
    // TODO: Open file and return handle for reading
    return nullptr;
}

void SD::closeFile(const std::string& filePath) {
    // TODO: Close opened file safely
}

std::vector<uint8_t> SD::readFileChunk(void* fileHandle) {
    // TODO: Read a chunk of data from the opened file
    return {};
}

void SD::handleRequest() {
    // TODO: Handle incoming file-related requests from control task via queue
}
