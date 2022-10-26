#pragma once
#include "ledc_rgb_led_t.h"


/**
  @brief Represents the @ref set_led_color_percent function of the  @ref rgb_ledc api.
 */
typedef void (*rgb_ledc__set_led_color_percent_func_t)(
    const struct ledc_rgb_led_t *led,
    int percent_red,
    int percent_green,
    int percent_blue,
    uint32_t fade_milliseconds);