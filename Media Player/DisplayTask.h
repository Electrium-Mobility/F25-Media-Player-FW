
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Basic playback state enumeration for display purposes
enum class PlaybackState {
    Stopped,
    Playing,
    Paused,
    Error
};

class DisplayTask {
public:
    // Attributes (from UML)
    QueueHandle_t queueHandle;
    std::string   currentTrack;
    PlaybackState playbackState;
    int           volumeLevel;
    int           batteryPercentage;
    bool          bluetoothConnected;

    // Functions (from UML)
    void updateTrack();
    void updatePlaybackStatus();
    void updateVolume();
    void updateBattery();
    void updateBluetooth();
    void handleMessage();
};
