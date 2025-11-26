#include "SDCardTask.h"
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
#define TAG "SDCardTask"

bool SDCardTask::mountSD() {
    esp_err_t ret;
    esp_vfs_fat_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 512,
        .disk_status_check_enable = false,
        .use_one_fat = true,
    };
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    // DARN, so compiling code in C++ vs in C will change certain things.
    // Before, you were allowed to use direct number conversions in the default configs
    // So for example the line ".d0 = 1," is valid as C will automatically convert that into
    // a gpio_num_t type. In C++, it will complain because 1 is an integer. Note that it will also
    // complain about other pins that you won't be using either because it's picky like that!
    // 
    slot_config.width = 1;
    ESP_LOGI(TAG, "Set SD bus width to 1");
     // Make sure you define this because in default config it is set to 0????
    sdmmc_host_t host_config = SDMMC_HOST_DEFAULT();
    sdmmc_card_t *card;
    const char parent_path[] = MOUNT_POINT; //Remember, an array variable is a POINTER to the first element
    ret = esp_vfs_fat_sdmmc_mount( // Passing a refernce to an array means double pointer
        parent_path, 
        &host_config, 
        &slot_config, 
        &mount_config, 
        &card // function will define card
    );

    if (ret != ESP_OK) {
        ESP_LOGI(TAG, "Mount failed: %d", ret);
        return false;
    }

    ESP_LOGI(TAG, "Mount Success: %d", ret);
    sdmmc_card_print_info(stdout, card); // This is why structs suck and classes are bettery
    // Anyways, seems like you can treat stdout as a FILE type?    

    return true;
}

std::vector<std::string> SDCardTask::listFiles() {
    // General procedure: open dir --> access/read dir --> close dir 
    // fopen() is from stdio.h, f_open() is from ff.h (FatFs module)
    // ff.h is automatically (implitly) included, my guess
    
    FF_DIR my_directory;
    FRESULT res;
    // Check out the all possible FRESULT return values (integers) and their meaning!
    
    res = f_opendir(&my_directory, "/"); // I thought you would pass "/sdcard", the MOUNT_POINT as the directory
    // But seems like FatFs Module does not recognize "/sdcard" as a true directory, but "/" is?
    // The alternative explanation is that the FatFs module is a high level library that simply just uses the directory
    // already set as the root directory (in this case esp_vfs_fats set it as "/sdcard"). It's just treating the sd card 
    // dir as base so it's trying to search for another "/sdcard" folder inside the real "/sdcard" dir.
    ESP_LOGI(TAG, "Opening Directory ErrNo: %d", res);

    FILINFO my_file;
    while ((res = f_readdir(&my_directory, &my_file)) == 0 && my_file.fname[0] != '\0') {
        // THe first argument checks for successful reads, the second checks for reaching end of directory

        WORD d = my_file.fdate; // The fdate field in FILINFO is encoded as not human readable
        // bits 0-4 are the days, bits 5-8 are the months, bits 9-15 are the years 
        int year  = ((d >> 9) & 0x7F) + 1980;
        int month = (d >> 5) & 0x0F;
        int day   = d & 0x1F;

        printf("[%d] %04d-%02d-%02d ", res, year, month, day); // Files created or modified by the esp chip have
        // dates naturally set to the year 1980, Jan 1

        // Why is it not printing the full name?
        // Because Long File Name is not enabled. Currently falls back to Short File Name
        // To enable, go to menuconfig and set LFN (heap and stack options)
        // You will notice that there's a hidden directory called System Volume Information that Windows PC cannot detect
        // It's hidden by OS but manages storage handling. Direct POSIX operations can still detect it. Just leave alone.
    
        if (my_file.fattrib & AM_DIR) { // Bit shift the attribute
            printf("* %s *\n", my_file.fname);
        } else {
            printf("%s\n", my_file.fname);
        }
        
        
    }
    
    //printf("Reading Directory ErrNo: %d\n", res); 

    res = f_closedir(&my_directory);
    ESP_LOGI(TAG, "Closing Directory ErrNo: %d", res);
    
    return {};
}

void* SDCardTask::openFile(const std::string& filePath) {
    // TODO: Open file and return handle for reading
    return nullptr;
}

void SDCardTask::closeFile(const std::string& filePath) {
    // TODO: Close opened file safely
}

std::vector<uint8_t> SDCardTask::readFileChunk(void* fileHandle) {
    // TODO: Read a chunk of data from the opened file
    return {};
}

void SDCardTask::handleRequest() {
    // TODO: Handle incoming file-related requests from control task via queue
}
