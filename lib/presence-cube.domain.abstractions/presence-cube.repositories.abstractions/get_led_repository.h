/**
 * @file get_led_repository.h
 * @brief accesspoint for platform specific RGB led informations
 */
#pragma once
#include "rgb_led_diode.h"
#include "get_led_repository_func.h"

/**
 * @brief Gets a list of rgb_led_diode_t objects
 * @return const rgb_led_domain_bag_t* if LEDs were found.
 * @return NULL if no LEDs were found.
 */
const rgb_led_domain_bag_t* get_led_repository__get_leds();

/**
 * @brief Finds a single led
 * @param id 
 * @return const rgb_led_diode_t* if the LED was found.
 * @return NULL if the LED could not be found.
 * @exception ERROR_CODE_ARGUMENT_NULL in case no LEDs are currently registered.
 */
const rgb_led_diode_t *get_led_repository__get_led(uint8_t id);