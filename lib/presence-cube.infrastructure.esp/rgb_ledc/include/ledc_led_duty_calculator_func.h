#pragma once
#include "stdint.h"
#include "ledc_led_t.h"

/**
  @brief Represents the @ref calculate_duty_percent function of the  @ref ledc_led_duty_calculator api.
 */
typedef float (*calculate_duty_percent_func_t)(const struct ledc_led_t *led);

/**
  @brief Represents the @ref calculate_duty_cycle function of the  @ref ledc_led_duty_calculator api.
 */
typedef uint32_t (*calculate_duty_cycle_func_t)(const struct ledc_led_t *led, uint8_t percent);