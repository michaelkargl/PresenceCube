#pragma once

/**
  @brief Represents the @ref rgb_ledc_adapter__set_color function of the  @ref rgb_ledc_adapter api.
 */
typedef void (*rgb_ledc_adapter__set_color_func_t)(
    const rgb_led_domain_t *led,
    uint8_t percent_red,
    uint8_t percent_green,
    uint8_t percent_blue);
