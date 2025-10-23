#include <string>
#include "esp_err.h"

// SystemInitializer
// Initializes system services required before any SD card or Bluetooth use.

enum class LogLevel {
    None,
    Error,
    Warn,
    Info,
    Debug,
    Verbose
};

class SystemInitializer {
public:
    // Attributes
    bool nvsInitialized;
    LogLevel logLevel;

    // Functions
    esp_err_t initNvs();
    void setLogLevel(LogLevel level);
    esp_err_t nvsFlashInit();
    void espLogLevelSet(const std::string& tag, LogLevel level);
};
