#pragma once
#include "stdint.h"
#include "ledc_led_t.h"

/**
  @brief Represents the @ref ledc_set_percent function of the  @ref ledc_facade api.
 */
typedef void (*ledc_set_percent_func_t)(const struct ledc_led_t *led, uint8_t percent, int fade_time_ms);