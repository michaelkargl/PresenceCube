#pragma once
#include "log_level.h"
#include "stdint.h"

void log_level_to_string(log_level level, char output_buffer[6], uint8_t output_buffer_size);