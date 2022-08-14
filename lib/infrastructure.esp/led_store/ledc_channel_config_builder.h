#pragma once
#include "hal/ledc_types.h"

ledc_channel_config_t build_ledc_channel_config(
    ledc_channel_t channel,
    ledc_timer_config_t timer,
    int gpio_pin
);