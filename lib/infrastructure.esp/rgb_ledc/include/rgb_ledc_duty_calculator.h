#pragma once
#include "stdint.h"
#include "ledc_led_t.h"

/**
 * @brief Get the maximum possible duty cycle for a specific LED configuration
 * @param led
 * @return uint32_t 
 */
uint32_t get_max_duty(const struct ledc_led_t *led);

/**
 * @brief Translates a percentage value to a LED specific duty cycle
 * 
 * @param led The led configuration to use as a basis of calculation
 * @param percent The percentage to translate into a duty cycle
 * @return uint32_t 
 */
uint32_t calculate_duty(const struct ledc_led_t *led, uint8_t percent);