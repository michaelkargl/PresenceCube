#pragma once
#include "ledc_rgb_led_t.h"
#include "esp_err.h"

esp_err_t led_store_initialize();

uint8_t led_store__get_led_count();

struct ledc_rgb_led_t* led_store__get_leds();
