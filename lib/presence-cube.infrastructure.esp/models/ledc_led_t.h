#pragma once
#include "hal/ledc_types.h"
#include "stdint.h"
#include "../models/led_type_t.h"

/**
 * @brief Represents a single esp32 LEDC Led
 * @param id uniquely identifies this instance within its parent scope
 * @param is_initialized helper property that helps the program determin if the struct itself
 *                       has been fully initialized or just contains a set of default values.
 * @param name A simple display name for this LED
 */
struct ledc_led_t
{
    bool is_initialized;
    uint8_t id;
    led_type_t led_type;
    char name[10];
    ledc_channel_config_t channel;
    ledc_timer_config_t timer;
};