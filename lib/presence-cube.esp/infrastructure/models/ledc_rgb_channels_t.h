#pragma once
#include "hal/ledc_types.h"

struct ledc_rgb_channels_t {
    ledc_channel_t red;
    ledc_channel_t green;
    ledc_channel_t blue;
};