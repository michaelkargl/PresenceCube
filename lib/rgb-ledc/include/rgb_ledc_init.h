#pragma once
#include "rgb_ledc_types.h"
#include "rgb_ledc_init_types.h"


/**
 * @brief Configures all RGB channel LEDS to be used with LED control
 * @param led 
 */
void configure_rgb_led(const struct ledc_rgb_led_t *led);


/**
 * @brief Configures a single LED to be used with LED control
 * @param led 
 */
void configure_led(const struct ledc_led_t *led);


/**
 * @brief Initializes a new ledc_rgb_led_t struct instance.
 * 
 * @param name A name that idendifies this ledc_rgb_led_t instance
 * @param _ledc_timer The timer that led control should use to control the LEDs
 * @param channels Ledc channel configurations for each RGB channel
 * @param pins Configures the pins each of the RGB channels are connected to
 * @param is_common_anode The type of RGB LED
 * @param fade_interval Specifies the fading interval between two colors in milliseconds.
 * @return struct ledc_rgb_led_t
 * @deprecated fade_interval This interval will be moved into the led_control service. It does not make much sense to hardwire fade intervals directly to the LED.
 */
struct ledc_rgb_led_t new_rgb_ledc_led(
    char name[10],
    ledc_timer_config_t _ledc_timer,
    const struct ledc_rgb_channels_t channels,
    const struct ledc_rgb_gpio_pins_t pins,
    bool is_common_anode,
    int32_t fade_interval
);