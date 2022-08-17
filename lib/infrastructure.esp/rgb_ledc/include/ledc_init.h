#pragma once
#include "ledc_led_t.h"
#include "esp_err.h"


/**
 * @brief configures the led to be used for LED control.
 *        i.e: sets up LEDC infrastructure for the specified led.
 * @exception UNINITIALIZED_DATA_ACCESS_ERROR if led is uninitialized
 * @param led
 */
void configure_led(const struct ledc_led_t *led);