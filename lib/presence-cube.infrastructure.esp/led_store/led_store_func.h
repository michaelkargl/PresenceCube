#pragma once
#include "ledc_rgb_led_t.h"

/**
  @brief Represents the @ref led_store__get_led function of the  @ref led_store api.
 */
typedef struct ledc_rgb_led_t *(*led_store__get_led_func_t)(uint8_t);