#pragma once
#include "stdint.h"
#include "ledc_led_t.h"

typedef void (*ledc_set_percent_func_t)(const struct ledc_led_t *led, uint8_t percent, int fade_time_ms);