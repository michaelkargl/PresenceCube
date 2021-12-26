#pragma once
#include "esp_err.h"

esp_err_t initialize_led_control();
const struct ledc_rgb_led_t* get_led_control_leds();
int get_led_control_initialized_led_count();