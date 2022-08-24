#pragma once
#include "led_domain.h"

typedef struct {
    led_domain_t *leds;
    uint8_t count;
} led_domain_bag_t;