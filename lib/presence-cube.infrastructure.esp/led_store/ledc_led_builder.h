#pragma once
#include "hal/ledc_types.h"
#include "ledc_led_t.h"

struct ledc_led_t build_ledc_led(
    char name[10],
    ledc_timer_config_t timer,
    ledc_channel_t channel,
    uint8_t gpio_pin,
    bool is_common_anode
);