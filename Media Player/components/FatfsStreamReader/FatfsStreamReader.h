/**
 * FATFSStreamReader - C skeleton (ESP-IDF/ESP-ADF friendly)
 * Creates an audio stream that reads data from files on FAT filesystem.
 *
 * Note: This is a pure-C skeleton. External types from ESP-ADF are kept opaque (void* or int enums).
 */
#ifndef FatfsStreamReader_H
#define FatfsStreamReader_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

struct FATFSStreamReader {
    void * cfg; /* fatfs_stream_cfg_t pointer (opaque to keep C-only skeleton). */
    void * handle; /* audio_element_handle_t (opaque). */
    int stream_type; /* audio_stream_type_t value; READER or WRITER from ESP-ADF. */
    bool mounted; /* Whether the FATFS is currently mounted. */
};

/* API */
void * FatfsStreamReader_init_stream(struct FATFSStreamReader *sr);
void FatfsStreamReader_set_stream_type(struct FATFSStreamReader *sr, int type);
int FatfsStreamReader_read_audio_data(struct FATFSStreamReader *sr, void *buffer, int bytes_to_read);
esp_err_t FatfsStreamReader_seek_position(struct FATFSStreamReader *sr, int position);
esp_err_t FatfsStreamReader_close_stream(struct FATFSStreamReader *sr);
void * FatfsStreamReader_init_host(void);
void * FatfsStreamReader_configure_slot(void);
esp_err_t FatfsStreamReader_mount_fatfs(struct FATFSStreamReader *sr, const char *path);
esp_err_t FatfsStreamReader_unmount_fatfs(struct FATFSStreamReader *sr);
void * FatfsStreamReader_get_card_info(struct FATFSStreamReader *sr);
int FatfsStreamReader_scan_files(const char *directory, char **out_list, int max_items);

#ifdef __cplusplus
}
#endif

#endif /* FatfsStreamReader_H */
