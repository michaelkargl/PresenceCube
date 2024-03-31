#pragma once
#include "stdint.h"

/**
 * @brief Adjusts the state of a LED with specified ID
 */
typedef struct {
    // The LED domain entity unique identifier
    uint8_t id;
    // The 8 bit RGB value for the red channel
    uint8_t red;
    // The 8 bit RGB value for the green channel
    uint8_t green;
    // The 8 bit RGB value for the blue channel
    uint8_t blue;
} set_led_command_t;