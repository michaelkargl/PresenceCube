#pragma once

// TODO: eventually move this dependency into the concrete implementation project
#include "driver/ledc.h"

struct ledc_rgb_gpio_pins_t {
    int red;
    int green;
    int blue;
};


// TODO: create a custom ledc_rgb_channels_t dto
// TODO: create mapping functionality to map between dto and domain object

struct ledc_rgb_channels_t {
    ledc_channel_t red;
    ledc_channel_t green;
    ledc_channel_t blue;
};