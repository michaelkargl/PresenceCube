#pragma once
#include "led/rgb_led_diode.h"

/**
  @brief Represents the @ref set_led_repository__update_led function of the  @ref set_led_repository api.
 */
typedef void (*set_led_repository__update_led_func_t)(
    uint8_t id,
    uint8_t red_percent,
    uint8_t green_percent,
    uint8_t blue_percent);