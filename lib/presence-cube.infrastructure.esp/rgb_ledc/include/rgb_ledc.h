#pragma once
#include "rgb_ledc_func.h"

/**
 * @brief Set the led color for a single LED
 *
 * @param led The led which color to be changed
 * @param percent_red Specifies the intensity of the red channel
 * @param percent_green Specifies the intensity of the green channel
 * @param percent_blue Specifies the intensity of the blue channel
 * @param fade_milliseconds the interval which a LED should take to fade softly between two colors.
 * @exception ERROR_CODE_ARGUMENT_NULL if the input LED was NULL.
 * @exception ERROR_CODE_UNINITIALIZED_DATA_ACCESS if the given input LED was uninitialized.
 */
void set_led_color_percent(
    const struct ledc_rgb_led_t *led,
    int percent_red,
    int percent_green,
    int percent_blue,
    uint32_t fade_milliseconds);

/**
 * @brief Set the led color for multiple LEDs
 *
 * @param leds An array of LEDs to act upon
 * @param leds_size The size of the LED array
 * @param percent_red Specifies the intensity of the red channel
 * @param percent_green Specifies the intensity of the green channel
 * @param percent_blue Specifies the intensity of the blue channel
 * @param fade_milliseconds the interval which a LED should take to fade softly between two colors.
 * @exception ERROR_CODE_ARGUMENT_NULL if the input LED was NULL.
 * @exception ERROR_CODE_UNINITIALIZED_DATA_ACCESS if any given input LED was uninitialized.
 */
void set_leds_color_percent(
    const struct ledc_rgb_led_t *leds,
    const int leds_size,
    int percent_red,
    int percent_green,
    int percent_blue,
    uint32_t fade_milliseconds);

/**
 * @brief Set the led color using 8bit color value
 *
 * @param led The led to set the color for
 * @param red red value (0-255)
 * @param green green value (0-255)
 * @param blue blue value (0-255)
 * @param fade_milliseconds the interval which a LED should take to fade softly between two colors.
 * @exception ERROR_CODE_ARGUMENT_NULL if the input LED was NULL.
 * @exception ERROR_CODE_UNINITIALIZED_DATA_ACCESS if any given input LED was uninitialized.
 */
void set_led_color_8bit(
    const struct ledc_rgb_led_t *led,
    uint8_t red,
    uint8_t green,
    uint8_t blue,
    uint32_t fade_milliseconds);