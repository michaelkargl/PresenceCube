#pragma once
#include "led_domain.h"

// TODO: move to global helper in case this proves useful
#define INITIALIZABLE_STRUCT bool is_initialized;

/**
 * @brief aggregate root for an RGB LED consisting of 3 channel leds
 */
typedef struct
{
    INITIALIZABLE_STRUCT;
    uint8_t id;
    char display_name[5];
    led_domain_t red;
    led_domain_t green;
    led_domain_t blue;
} rgb_led_domain_t;

/**
 * @brief aggregate root for an RGB LED diode
 */
typedef struct {
    INITIALIZABLE_STRUCT;
    rgb_led_domain_t *leds;
    uint8_t count;
} rgb_led_domain_bag_t;
