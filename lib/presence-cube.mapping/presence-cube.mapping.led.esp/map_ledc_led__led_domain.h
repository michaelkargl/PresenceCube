#pragma once
#include "led_domain_bag.h"
#include "ledc_led_t.h"


/**
 * @brief Maps the given @ref ledc_led_t to its domain representative @ref led_domain_t.
 * 
 * @param led the led element to map.
 * @return led_domain_t a new @ref led_domain_t mapping of @ref led.
 * @exception ERROR_CODE_ARGUMENT_NULL if the input @ref led is NULL.
 */
led_domain_t map_ledc_led__to__led_domain(const struct ledc_led_t *led);


/**
 * @brief Maps all elements of @ref source_leds to the @ref target_leds bag.
 * 
 * @param source_leds an array of @ref ledc_led_t elements to map.
 * @param source_led_count the 1-based length of the @ref source_leds array.
 * @param target_leds a buffer target bag that will receive the mappings of @ref source_leds. It must be at least the of size @ref source_led_count.
 * @param target_led_count the 1-based size of the @ref target_leds.
 * 
 * @exception ERROR_CODE_ARGUMENT_NULL if either @ref source_leds or @ref target_led_bag is NULL.
 * @exception ERROR_CODE_INSUFFICIENT_BUFFER if the @ref target_led_bag is not big enough to hold all the mappings of @ref source_leds.
 */
void map_ledc_led_array__to__led_domain_bag(
    const struct ledc_led_t source_leds[], uint8_t source_led_count,
    led_domain_bag_t *target_led_bag
);