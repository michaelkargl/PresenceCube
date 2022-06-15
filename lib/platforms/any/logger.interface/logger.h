#pragma once
#include "log_level.h"

void log_information(const char* context_name, const char* message_format, ...);
void log_debug(const char* context_name, const char* message_format, ...);