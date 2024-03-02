#pragma once
#include "rgb_led_diode.h"

/**
  @brief Represents the @ref get_led_repository__get_led function of the  @ref get_led_repository api.
 */
typedef const rgb_led_diode_t *(*get_led_repository__get_led_func_t)(uint8_t id);

/**
  @brief Represents the @ref get_led_repository__get_leds function of the  @ref get_led_repository api.
 */
typedef const rgb_led_domain_bag_t *(*get_led_repository__get_leds_func_t)();