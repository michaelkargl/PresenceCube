#pragma once
#include "ledc_rgb_led_t.h"
#include "esp_err.h"
#include "led_store_func.h"

/**
 * @brief Triggers module initialization.
 * @return esp_err_t
 */
esp_err_t led_store__initialize();

/**
 * @brief Returns the current module initialization state.
 *
 * @return true if module is fully initialized.
 * @return false if module is uninitialized.
 */
bool led_store__initialized();

/**
 * @brief Retrieves the amount of registered leds / the size of
 *        the array returned by @ref led_store__get_leds
 * @return uint8_t the LED count.
 */
uint8_t led_store__get_led_count();

/**
 * @brief Retrieves a list of registered leds.
 *        To find out it's length, use the provided
 *        @ref led_store__get_led_count function.
 * @return struct ledc_rgb_led_t* an array of RGB LEDs.
 * @return NULL if no RGB LEDs were found.
 */
struct ledc_rgb_led_t *led_store__get_leds();

/**
 * @brief Finds a single identifiable LED.
 *
 * @param id the LEDs identifier.
 * @return struct ledc_rgb_led_t* a reference to the LED.
 * @return NULL if no RGB LED with id could be found.
 * @exception ERROR_CODE_UNINITIALIZED_MODULE_ACCESS if the module has not been initialized.
 * @exception ERROR_CODE_ARGUMENT_NULL if no LEDs where
 *            registered in the system at all / the refernce
 *            to the list of registered LEDs was NULL.
 */
struct ledc_rgb_led_t *led_store__get_led(uint8_t id);

/**
 * @brief Updates the state of a given LED.
 * @param led A modified LED instance.
 * @exception ERROR_CODE_RESOURCE_NOT_FOUND if led with id could not be found.
 * @exception ERROR_CODE_UNINITIALIZED_MODULE_ACCESS if the module has not been initialized.
 */
void led_store__update(struct ledc_rgb_led_t led);