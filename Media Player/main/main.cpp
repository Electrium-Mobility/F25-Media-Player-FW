// NOTE
// Using CPP files require add on materials such as [extern "C"] or [__cplusplus]
// since ESP-IDF is C based
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int app_main();

#ifdef __cplusplus
}
#endif

int app_main(void)
{
    printf("Hello World!\n");
    return 0;
}