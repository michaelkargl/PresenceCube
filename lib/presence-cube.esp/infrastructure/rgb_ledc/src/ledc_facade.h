#pragma once
#include "hal/ledc_types.h"
#include "ledc_facade_funcs.h"

void ledc_facade__set_percent(const struct ledc_led_t *led, uint8_t percent, int fade_time_ms);
