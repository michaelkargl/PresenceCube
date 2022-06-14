#pragma once
#include "log_level.h"

void log_message(log_level level, const char* context_name, const char* message);
void log_information(const char* context_name, const char* message);
void log_debug(const char* context_name, const char* message);