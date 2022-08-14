#pragma once
#include "ledc_led_t.h"

/**
 * @brief Aggregate root for 3 LEDs representing a single RGB-LED.
 * @param id uniquely identifies this instance within its parent scope
 * @param is_initialized helper property that helps the program determin if the struct itself
 *                       has been fully initialized or just contains a set of default values.
 */
struct ledc_rgb_led_t  
{
    bool is_initialized;
    uint8_t id;
    char name[10];
    struct ledc_led_t red;
    struct ledc_led_t green;
    struct ledc_led_t blue;
};