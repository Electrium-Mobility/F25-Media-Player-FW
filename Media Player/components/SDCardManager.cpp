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

using SD = SDCardManager;
/**
 * @brief Mounts the SD Card Volume as VFS. Sets up required sdmmc interface.
 * @todo requires better error handling and probing. When board is brought up, make sure to use card detect
 * @return a boolean indicating whether the VFS FatFs creation was succesful
 */
bool SD::mountSD() {
    SD::sdMountStatus = false;
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
        &(SD::card) // function will define card
    );

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Mount failed: %d", ret);
        return false;
    }

    ESP_LOGI(TAG, "Mount Success: %d", ret);
    sdmmc_card_print_info(stdout, SD::card); // This is why structs suck and classes are bettery
    // Anyways, seems like you can treat stdout as a FILE type?    
    SD::sdMountStatus = true;
    return true;
}


/**
 * @brief Decodes a bitwise date from FILINFO. Returns the value MMDDYYYY to the OUT parameter
 * @param date a 16 bit WORD coming from a FILINFO data type 
 * @param out a c-style string of where the decoded value should be overwritten to. Should be at least 16 bytes
 */
void decodeDate(WORD date, char *out) {
    WORD d = date; 
    snprintf(out, 16, "%02d-%02d-%04d",
        (int) ((d >> 5) & 0x0F),            // Month
        (int) (d & 0x1F),                    // Day
        (int) (((d >> 9) & 0x7F) + 1980)   // Year
    );
}

/**
 * @brief Updates fileList with all present files listed in the path directory in the order
 * presented by the SD Card file system. This is considered unsorted
 * @param path a c-style string of the directory to be read
 */
void SD::listFilesFromCard(const char *path) {
    SD::fileList.clear(); // Remove information from previous read

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
    ESP_LOGI(TAG, "Opening Directory ErrNo: %d", res);
    if (res != 0) {
        ESP_LOGE(TAG, "Failed to open path. Path probably doesn't exist");
    }

    FILINFO myFile;
    char filePrintInfo[299 + 1]; 
    char date[16];
    while ((res = f_readdir(&myDirectory, &myFile)) == 0 && myFile.fname[0] != '\0') {
        // The first argument checks for successful reads, the second checks for reaching end of directory
       
        decodeDate(myFile.fdate, date);// The fdate field in FILINFO is encoded as not human readable
        // bits 0-4 are the days, bits 5-8 are the months, bits 9-15 are the years 
        
        // WE HAVE AN ISSUE
        // 1. filePrintInfo is previously 256 sized, but the LFN is also 256, so you'll have to forcibly truncate the file name as you write extra char
        //    Additionally, the snprintf function requires 255 (previously) but your LFN name is 256, is compile error
        // 2. Imagine that we have 300 songs in a single directory. Each song is 256 char, so = 76800 bytes
        //    Based off of the memory mapping produced every build having aroud 300k-ish bytes left, we should be careful!
        snprintf(filePrintInfo, 299, "[%d] %s %s %s", 
            res, 
            date,
            myFile.fname,
            (myFile.fattrib & AM_DIR) ? "**" : ""
        );
        printf("%s\n", filePrintInfo); 

        std::string s = filePrintInfo;
        SD::fileList.push_back(s);

        // Why is it not printing the full name?
        // Because Long File Name is not enabled. Currently falls back to Short File Name
        // To enable, go to menuconfig and set LFN (heap and stack options)
        // You will notice that there's a hidden directory called System Volume Information that Windows PC cannot detect
        // It's hidden by OS but manages storage handling. Direct POSIX operations can still detect it. Just leave alone.
    }
    res = f_closedir(&myDirectory);
    ESP_LOGI(TAG, "Closing Directory ErrNo: %d", res);
}

/**
 * @brief Prints all files as is currently in the listed directory
 * @param path a constant c-style string pointer. Pass in "/" for current directory,
 * or pass in "/My directory" or "My directory". The "/" is unnecessary.
 */
void SD::showFileList(const char *path) {
    ESP_LOGI(TAG, "Checking DIR");
    listFilesFromCard(path);
    for (std::string s : fileList) {
        printf("%s\n", s.c_str());
    }
    printf("# Files: %d\nTotal Capacity: %d\n", SD::fileList.size(), SD::fileList.capacity());

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
