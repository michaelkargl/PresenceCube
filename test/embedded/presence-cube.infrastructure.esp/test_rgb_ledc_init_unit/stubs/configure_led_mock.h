#pragma once
#include "ledc_led_t.h"

void configure_led_mock__reset();

uint8_t configure_led_mock__get_callcount();

void configure_led_mock(const struct ledc_led_t* led);