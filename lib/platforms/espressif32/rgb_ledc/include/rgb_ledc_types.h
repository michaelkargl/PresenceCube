#pragma once

// TODO: eventually move this dependency into the concrete implementation project
#include "driver/ledc.h"

// TODO: create custom ledc_channel_config_t dto
// TODO: create custom ledc_timer_config_t dto
// TODO: create mapping helper to map between dto and domain object

struct ledc_led_t
{
    char name[10];
    ledc_channel_config_t channel;
    ledc_timer_config_t timer;
};

struct ledc_rgb_led_t  
{
    bool is_initialized;
    bool is_common_anode;
    char name[10];
    struct ledc_led_t red;
    struct ledc_led_t green;
    struct ledc_led_t blue;
    int32_t fade_milliseconds;
};