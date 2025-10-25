#pragma once
#include "ledc_rgb_led_t.h"


/**
 * @brief Configures all RGB channel LEDS to be used with LED control
 * @param led 
 */
void configure_rgb_led(const struct ledc_rgb_led_t *led);


/**
 * @brief Enables a list of leds to be used with LED control
 * @param leds a pointer to an array of leds
 * @param led_count the length of the leds array
 */
void configure_rgb_leds(const struct ledc_rgb_led_t *leds, uint8_t led_count);