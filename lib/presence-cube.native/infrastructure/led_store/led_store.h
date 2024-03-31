#pragma once
#include "led/rgb_led_diode.h"
#include "string.h"
#include "led_store_func.h"

/**
 * @brief Retrieves the entirety of registered LEDs.
 *
 * @return const rgb_led_diode_bag_t* a bag filled with LEDs.
 */
const rgb_led_diode_bag_t *led_store__get_leds();

/**
 * @brief Attempts to find a single identifiable LED
 *
 * @param id the LEDs identity
 * @return const rgb_led_diode_t* pointer to the found LED instance.
 * @return NULL if LED was unable to be found.
 */
const rgb_led_diode_t *led_store__get_led(uint8_t id);

/**
 * @brief Updates the state of a given LED.
 * @param led A modified LED instance.
 * @exception ERROR_CODE_RESOURCE_NOT_FOUND if led with id could not be found.
 */
void led_store__update(rgb_led_diode_t led);

/**
 * @brief Returns the current module initialization state.
 *
 * @return true if module is fully initialized.
 * @return false if module is uninitialized.
 */
bool led_store__initialized();

/**
 * @brief Triggers module initialization.
 */
void led_store__initialize();