#pragma once
#include "led_diode.h"

typedef struct {
    led_diode_t *leds;
    uint8_t count;
} led_domain_bag_t;