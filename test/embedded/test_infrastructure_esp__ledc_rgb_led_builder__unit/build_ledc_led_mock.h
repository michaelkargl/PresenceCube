#pragma once
#include "ledc_led_builder.h"

void build_ledc_led_mock__reset();

uint8_t build_ledc_led_mock__get_callcount();

struct ledc_led_t build_ledc_led_mock__build_ledc_led(
    char name[10],
    ledc_timer_config_t timer,
    ledc_channel_t channel,
    uint8_t gpio_pin,
    led_type_t led_type
);