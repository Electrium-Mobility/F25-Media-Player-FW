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

#ifdef __cplusplus
extern "C" {
#endif

class MusicPlayer {
public:

    char current_track[256]; /* Path to the currently playing file. */
    float volume; /* Current volume 0-1. */
    bool is_playing; /* Playback status. */
    int play_mode; /* Play mode enum (normal, shuffle, repeat). */
    i2s_chan_handle_t tx_handle;

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
};
#ifdef __cplusplus
}
#endif

#endif /* MUSIC_PLAYER_H */
