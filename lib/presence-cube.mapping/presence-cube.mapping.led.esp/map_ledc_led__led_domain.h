#pragma once
#include "led_domain.h"
#include "ledc_led_t.h"

led_domain_t map_ledc_led__to__led_domain(struct ledc_led_t *led);