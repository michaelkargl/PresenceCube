#pragma once
#include "ledc_led_duty_calculator_func.h"

// TODO: prefix those functions with `ledc_led_duty_calculator`!
//       Mind that theres function pointer types assigned for testing (ledc_led_duty_calculator_func.h)

/**
 * @brief Translates a percentage value to a LED specific duty cycle
 *
 * @param led The led configuration to use as a basis of calculation
 * @param percent The percentage to translate into a duty cycle
 * @return uint32_t a duty cycle in the intensity of the specified percentage
 * @exception ERROR_CODE_ARGUMENT_NULL if the input LED was NULL.
 * @exception ERROR_CODE_UNINITIALIZED_DATA_ACCESS if the given input LED was uninitialized.
 */
uint32_t ledc_led_duty_calculator__calculate_duty_cycle(const struct ledc_led_t *led, uint8_t percent);

/**
 * @brief Gets the current output percentage for a given led (this is the inverse of ledc_led_duty_calculator__calculate_duty_cycle).
 * @return float percentage reflecting the leds current brightness level.
 * @exception ERROR_CODE_ARGUMENT_NULL if the input LED was NULL.
 * @exception ERROR_CODE_UNINITIALIZED_DATA_ACCESS if the given input LED was uninitialized.
 */
float ledc_led_duty_calculator__calculate_brightness_percent(const struct ledc_led_t *led);