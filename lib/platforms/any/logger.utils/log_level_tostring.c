#include "log_level_tostring.h"
#include "stdio.h"

void log_level_to_string(log_level level, char output_buffer[6], uint8_t output_buffer_size)
{
    switch (level)
    {
    case Information_log_level:
    {
        snprintf(output_buffer, output_buffer_size, "%s", "INFO");
        return;
    }
    case Debug_log_level:
    {
        snprintf(output_buffer, output_buffer_size, "%s", "DEBUG");
        return;
    }
    case Warning_log_level:
    {
        snprintf(output_buffer, output_buffer_size, "%s", "WARN");
        return;
    }
    case Error_log_level:
    {
        snprintf(output_buffer, output_buffer_size, "%s", "ERROR");
        return;
    }
    default:
    {
        output_buffer = 0;
        return;
    }
    }
}