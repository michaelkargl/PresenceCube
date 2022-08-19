#pragma once
#include "hal/ledc_types.h"
#include "ledc_led_t.h"

void ledc_set_percent(const struct ledc_led_t *led, uint8_t percent, int fade_time_ms);