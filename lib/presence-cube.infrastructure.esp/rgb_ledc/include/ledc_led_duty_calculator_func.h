#pragma once
#include "stdint.h"
#include "ledc_led_t.h"

typedef float (*calculate_duty_percent_func_t)(const struct ledc_led_t *led);

typedef uint32_t (*calculate_duty_cycle_func_t)(const struct ledc_led_t *led, uint8_t percent);