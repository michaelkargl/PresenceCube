#include "logger.h"
#include "esp_log.h"

void log_information(const char* context_name, const char* message_format, ...) {
    va_list variadic_args;
    va_start(variadic_args, message_format);

    esp_log_writev(ESP_LOG_INFO, context_name, message_format, variadic_args);

    va_end(variadic_args);
}

void log_debug(const char* context_name, const char* message_format, ...) {
    va_list variadic_args;
    va_start(variadic_args, message_format);

    esp_log_writev(ESP_LOG_DEBUG, context_name, message_format, variadic_args);

    va_end(variadic_args);
}