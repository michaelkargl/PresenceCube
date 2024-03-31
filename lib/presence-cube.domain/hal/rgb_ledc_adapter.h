#pragma once
#include "led/rgb_led_diode.h"
#include "rgb_ledc_adapter_func.h"

/**
 * @brief Initializes the module.
 */
void rgb_ledc_adapter__initialize();

/**
 * @brief Retrieves the modules' initialization status.
 * @return true
 * @return false
 */
bool rgb_ledc_adapter__initialized();

/**
 * @brief Alters the color state of an LED.
 *
 * @param led The LED whose state should be changed.
 * @param percent_red The RGB red channel value in percent.
 * @param percent_green The RGB green channel value in percent.
 * @param percent_blue The RGB blue channel value in percent.
 * @exception ERROR_CODE_ARGUMENT_NULL in case of NULL input references.
 * @exception ERROR_CODE_UNINITIALIZED_MODULE_ACCESS if the module has not been initialized.
 */
void rgb_ledc_adapter__set_color(
    const rgb_led_diode_t *led,
    uint8_t percent_red,
    uint8_t percent_green,
    uint8_t percent_blue);
