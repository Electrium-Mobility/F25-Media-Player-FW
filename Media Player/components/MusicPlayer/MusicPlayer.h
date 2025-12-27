/**
 * MusicPlayer - C skeleton (ESP-IDF/ESP-ADF friendly)
 * Main application controller that manages music playback.
 *
 * Note: This is a pure-C skeleton. External types from ESP-ADF are kept opaque (void* or int enums).
 */
#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>
#include <string.h>
#include <driver/i2s_std.h>
#include <driver/i2c_master.h>

#ifdef __cplusplus
extern "C" {
#endif

struct WM8731RegisterDefaults {
    uint8_t addr_0E[2] = {0x0, 0b10010111};
    uint8_t addr_1E[2] = {0x1, 0b10010111};
    uint8_t addr_2E[2] = {0x2, 0b01111001};
    uint8_t addr_3E[2] = {0x3, 0b01111001};
    uint8_t addr_4E[2] = {0x4, 0b00001010};
    uint8_t addr_5E[2] = {0x5, 0b00001000};
    uint8_t addr_6E[2] = {0x6, 0b10011111};
    uint8_t addr_7E[2] = {0x7, 0b00001010};
    uint8_t addr_8E[2] = {0x8, 0b00000000};
    uint8_t addr_9E[2] = {0x9, 0b00000000};
    uint8_t addr_FE[2] = {0xF, 0b00000000};
};

/**
 * Be careful of little-big endian stuff, you might need to change to uint16_t
 * In bytes 1:0, the first 7 bits are address and the last 9 bits are data
 * You will need to bit shift.
 */
struct WM8731RegisterCustom {
    uint8_t addr_0E[2] =        {0x00 << 1, 0b10010111}; // Line In
    uint8_t addr_1E[2] =        {0x01 << 1, 0b10010111}; // Line In
    uint8_t addr_2E[2] =        {0x02 << 1, 0b01011001}; // HP Out
    uint8_t addr_3E[2] =        {0x03 << 1, 0b01011001}; // HP Out
    uint8_t addr_4E[2] =        {0x04 << 1, 0b00010010};
    uint8_t addr_5E[2] =        {0x05 << 1, 0b00010000}; // Disable de-emphasis
    uint8_t addr_5E_44k1[2] =   {0x05 << 1, 0b00010100};
    uint8_t addr_5E_48k[2] =    {0x05 << 1, 0b00010110}; 
    uint8_t addr_6E[2] =        {0x06 << 1, 0b01100111}; // Power Down Toggle
    uint8_t addr_7E[2] =        {0x07 << 1, 0b00000010};
    uint8_t addr_8E_44k1[2] =   {0x08 << 1, 0b00100000};
    uint8_t addr_8E_48k[2] =    {0x08 << 1, 0b00000000};
    uint8_t addr_9E_daiON[2] =  {0x09 << 1, 0b00000001};
    uint8_t addr_9E_daiOFF[2] = {0x09 << 1, 0b00000000};
    uint8_t addr_FE[2] =        {0x0F << 1, 0b00000000}; // Reset
};

class MusicPlayer {
public:

    char current_track[256]; /* Path to the currently playing file. */
    float volume; /* Current volume 0-1. */
    bool is_playing; /* Playback status. */
    int play_mode; /* Play mode enum (normal, shuffle, repeat). */
    i2s_chan_handle_t tx_handle;
    i2c_master_dev_handle_t dev_handle;
    i2c_master_bus_handle_t bus_handle;
    uint8_t wm8731Addr = 0b0011010;

    /* API */
    MusicPlayer();
    esp_err_t music_player_play(struct MusicPlayer *mp, const char *track_path);
    esp_err_t music_player_pause(struct MusicPlayer *mp);
    esp_err_t music_player_stop(struct MusicPlayer *mp);
    esp_err_t music_player_resume(struct MusicPlayer *mp);
    esp_err_t music_player_next_track(struct MusicPlayer *mp);
    esp_err_t music_player_previous_track(struct MusicPlayer *mp);
    void music_player_set_volume(struct MusicPlayer *mp, int level);
    void music_player_shuffle(struct MusicPlayer *mp);
    void music_player_repeat(struct MusicPlayer *mp);
    void music_player_get_status(struct MusicPlayer *mp, char *out_buf, int out_len);
    void testWavAudioI2S(const char *file);
    void testMP3AudioI2S(const char *file);
    bool testReconfigI2S(uint32_t freq, int bitsPerSample, int channels);
    void testInitI2S();
    void testCloseI2S();
    void testRunI2C();
    void testCloseI2C();
};
#ifdef __cplusplus
}
#endif

#endif /* MUSIC_PLAYER_H */
