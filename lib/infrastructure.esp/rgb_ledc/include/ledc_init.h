#pragma once
#include "ledc_led_t.h"


/**
 * @brief configures the led to be used for LED control.
 *        i.e: sets up LEDC infrastructure for the led.
 * @param led
 */
void configure_led(const struct ledc_led_t *led);