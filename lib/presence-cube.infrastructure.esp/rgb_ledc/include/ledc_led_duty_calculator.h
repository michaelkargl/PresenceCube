#pragma once
#include "stdint.h"
#include "ledc_led_t.h"
#include "ledc_led_duty_calculator_func.h"


/**
 * @brief Translates a percentage value to a LED specific duty cycle
 * 
 * @param led The led configuration to use as a basis of calculation
 * @param percent The percentage to translate into a duty cycle
 * @return uint32_t 
 */
uint32_t calculate_duty_cycle(const struct ledc_led_t *led, uint8_t percent);


/**
 * @brief Gets the current output percentage for a given led.
 * @return float percentage
 */
float calculate_duty_percent(const struct ledc_led_t *led);