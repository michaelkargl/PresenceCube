#include "logger.h"
#include "esp_log.h"


static void log_message(
    esp_log_level_t log_level,
    const char* log_level_string,
    const char *context_name,
    const char *message_format,
    va_list variadic_message_args
) {
    esp_log_write(log_level, context_name, "[%s %s]: ", log_level_string, context_name);
    esp_log_writev(log_level, context_name, message_format, variadic_message_args);
}

void log_information(const char *context_name, const char *message_format, ...)
{
    va_list variadic_args;
    va_start(variadic_args, message_format);
    {
        log_message(ESP_LOG_INFO, "INFO", context_name, message_format, variadic_args);
    }
    va_end(variadic_args);
}

void log_debug(const char *context_name, const char *message_format, ...)
{
    va_list variadic_args;
    va_start(variadic_args, message_format);
    {
        log_message(ESP_LOG_DEBUG, "DEBUG", context_name, message_format, variadic_args);
    }
    va_end(variadic_args);
}

void log_warning(const char *context_name, const char *message_format, ...)
{
    va_list variadic_args;
    va_start(variadic_args, message_format);
    {
        log_message(ESP_LOG_WARN, "WARN", context_name, message_format, variadic_args);
    }
    va_end(variadic_args);
}

void log_error(const char *context_name, const char *message_format, ...)
{
    va_list variadic_args;
    va_start(variadic_args, message_format);
    {
        log_message(ESP_LOG_ERROR, "ERROR", context_name, message_format, variadic_args);
    }
    va_end(variadic_args);
}