#pragma once
#include "stdint.h"

/**
 * @brief Response object of the @ref set_led_command_handler
 */
typedef struct {
    // Unique identifier of the modified LED
    uint8_t id;
} set_led_command_response_t;