#pragma once
#include "led_domain.h"
#include "ledc_led_t.h"

led_domain_t map_ledc_led__to__led_domain(const struct ledc_led_t *led);

void map_ledc_led_array__to__led_domain_array(
    const struct ledc_led_t source_leds[], uint8_t source_led_count,
    led_domain_t target_leds[], uint8_t target_led_count
);