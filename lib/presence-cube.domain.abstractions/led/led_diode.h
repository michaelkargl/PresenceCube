#pragma once

#include "stdint.h"
#include "stdbool.h"

#define LED_DIODE_T__DISPLAY_NAME__BUFFER_SIZE 5

typedef struct {
    bool is_initialized;
    uint8_t id;
    uint8_t brightness_percent;
    char display_name[LED_DIODE_T__DISPLAY_NAME__BUFFER_SIZE];
} led_diode_t;



