#include "logger.h"
#include "esp_log.h"


void log_message(log_level level, const char* context_name, const char* message) {
    switch(level) {
        case Information_log_level: {
            return log_information(context_name, message);
        }
        case Debug_log_level: {
            return log_debug(context_name, message);
        }
        default: {
            // TODO: log error if this happens
            log_information(context_name, "Unable to resolve given log level, assuming debug.");
            return log_debug(context_name, message);
        }
    }
}

void log_information(const char* context_name, const char* message) {
    ESP_LOGI(context_name, "%s", message);
}

void log_debug(const char* context_name, const char* message) {
    ESP_LOGD(context_name, "%s", message);
}