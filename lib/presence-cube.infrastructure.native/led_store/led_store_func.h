#pragma once
#include "rgb_led_diode.h"

/**
  @brief Represents the @ref led_store__get_leds function of the  @ref led_store api.
 */
typedef const rgb_led_domain_bag_t *(*led_store__get_leds_func_t)();

/**
  @brief Represents the @ref led_store__get_led function of the  @ref led_store api.
 */
typedef const rgb_led_diode_t *(*led_store__get_led_func_t)(uint8_t id);

/**
  @brief Represents the @ref led_store__update function of the  @ref led_store api.
 */
typedef void (*led_store__update_func_t)(rgb_led_diode_t led);