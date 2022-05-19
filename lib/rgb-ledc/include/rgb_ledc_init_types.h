#pragma once
#include "driver/ledc.h"

struct ledc_rgb_gpio_pins_t {
    int red;
    int green;
    int blue;
};

struct ledc_rgb_channels_t {
    ledc_channel_t red;
    ledc_channel_t green;
    ledc_channel_t blue;
};