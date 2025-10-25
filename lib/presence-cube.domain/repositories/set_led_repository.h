#pragma once
#include "set_led_repository_func.h"

/**
 * @brief Updates the values of a specified LED.
 *
 * @param id the identifier of the LED to modify.
 * @param red RGB red channel percentage.
 * @param green RGB green channel percentage.
 * @param blue RGB blue channel percentage.
 * @exception ERROR_CODE_RESOURCE_NOT_FOUND if a LED with specified id could not be found.
 */
void set_led_repository__update(
    uint8_t id,
    uint8_t red,
    uint8_t green,
    uint8_t blue);