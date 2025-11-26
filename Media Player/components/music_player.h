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

#ifdef __cplusplus
extern "C" {
#endif

struct MusicPlayer {
    char current_track[256]; /* Path to the currently playing file. */
    int volume; /* Current volume 0-100. */
    bool is_playing; /* Playback status. */
    int play_mode; /* Play mode enum (normal, shuffle, repeat). */
};

/* API */
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

#ifdef __cplusplus
}
#endif

#endif /* MUSIC_PLAYER_H */
