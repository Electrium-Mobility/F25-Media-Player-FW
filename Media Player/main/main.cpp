// NOTE
// Using CPP files require add on materials such as [extern "C"] or [__cplusplus]
// since ESP-IDF is C based
#include <stdio.h>


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

void alternate_task(void) {
    SDCardManager card_module = SDCardManager{};
    printf("Mount Success: %b\n", card_module.mountSD());
    card_module.showFileList("/");
    card_module.showFileList("/My Folder");
    card_module.showFileList("My Folder");
    card_module.showFileList("Error");
}

int app_main(void)
{
    printf("Hello World!\n");
    if (TESTING_MODE) {
        alternate_task();
    }
    return 0;
}