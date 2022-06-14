#include "log_level_tostring.h"
#include "logger.h"
#include "stdio.h"

void log_message(log_level level, const char* context_name, const char* message) {
    char level_string[6];
    log_level_to_string(level, level_string, sizeof(level_string));
    printf("[%s %s]: %s\n", context_name, level_string, message);
}

void log_information(const char* context_name, const char* message) {
    log_message(Information_log_level, context_name, message); 
}

void log_debug(const char* context_name, const char* message) { 
    log_message(Debug_log_level, context_name, message);
}