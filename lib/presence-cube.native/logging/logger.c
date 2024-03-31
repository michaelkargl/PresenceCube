#include "log_level_tostring.h"
#include "logger.iface.h"
#include "stdio.h"
#include "stdarg.h"

/**
 * @brief prints the context portion of a log message
 * @return the hara
 * @example "[main: INFO]: "
 */
static void print_log_context(log_level level, const char *log_context)
{
    char level_string[6];
    log_level_to_string(level, level_string, sizeof(level_string));
    printf("[%s %s]: ", log_context, level_string);
}

static void log_message(
    log_level level,
    const char *log_context,
    const char *message_format,
    va_list variadic_arguments)
{
    print_log_context(level, log_context);
    vprintf(message_format, variadic_arguments);
}

void log_information(const char *context_name, const char *message_format, ...)
{
    va_list message_args;
    va_start(message_args, message_format);
    {
        log_message(Information_log_level, context_name, message_format, message_args);
    }
    va_end(message_args);
}

void log_debug(const char *context_name, const char *message_format, ...)
{
    va_list message_args;
    va_start(message_args, message_format);
    {
    log_message(Debug_log_level, context_name, message_format, message_args);
    }
    va_end(message_args);
}

void log_warning(const char *context_name, const char *message_format, ...) {
    va_list message_args;
    va_start(message_args, message_format);
    {
        log_message(Warning_log_level, context_name, message_format, message_args);
    }
    va_end(message_args);
}

void log_error(const char* context_name, const char *message_format, ...) {
    va_list variadic_args;
    va_start(variadic_args, message_format);
    {
        log_message(Error_log_level, context_name, message_format, variadic_args);
    }
    va_end(variadic_args);
}