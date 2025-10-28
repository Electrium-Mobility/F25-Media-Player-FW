/**
 * FATFSStreamReader - C skeleton (ESP-IDF/ESP-ADF friendly)
 * Creates an audio stream that reads data from files on FAT filesystem.
 *
 * Note: This is a pure-C skeleton. External types from ESP-ADF are kept opaque (void* or int enums).
 */
#ifndef FATFS_STREAM_READER_H
#define FATFS_STREAM_READER_H

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
void * fatfs_stream_reader_init_stream(struct FATFSStreamReader *sr);
void fatfs_stream_reader_set_stream_type(struct FATFSStreamReader *sr, int type);
int fatfs_stream_reader_read_audio_data(struct FATFSStreamReader *sr, void *buffer, int bytes_to_read);
esp_err_t fatfs_stream_reader_seek_position(struct FATFSStreamReader *sr, int position);
esp_err_t fatfs_stream_reader_close_stream(struct FATFSStreamReader *sr);
void * fatfs_stream_reader_init_host(void);
void * fatfs_stream_reader_configure_slot(void);
esp_err_t fatfs_stream_reader_mount_fatfs(struct FATFSStreamReader *sr, const char *path);
esp_err_t fatfs_stream_reader_unmount_fatfs(struct FATFSStreamReader *sr);
void * fatfs_stream_reader_get_card_info(struct FATFSStreamReader *sr);
int fatfs_stream_reader_scan_files(const char *directory, char **out_list, int max_items);

#ifdef __cplusplus
}
#endif

#endif /* FATFS_STREAM_READER_H */
