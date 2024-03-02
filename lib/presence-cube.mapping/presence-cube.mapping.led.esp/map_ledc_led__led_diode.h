#pragma once
#include "led_diode_bag.h"
#include "rgb_led_diode.h"
#include "ledc_rgb_led_t.h"

/**
 * @brief Maps the given @ref ledc_led_t to its domain representative @ref led_diode_t.
 * 
 * @param led the led element to map.
 * @return led_diode_t a new @ref led_diode_t mapping of @ref led.
 * @exception ERROR_CODE_ARGUMENT_NULL if the input @ref led is NULL.
 */
led_diode_t map_ledc_led__to__led_diode(const struct ledc_led_t *led);


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
void map_ledc_led_array__to__led_diode_bag(
    const struct ledc_led_t source_leds[], uint8_t source_led_count,
    led_diode_bag_t *target_led_bag
);



/**
 * @brief Maps the given source led to a target of type @ref rgb_led_diode_t
 * 
 * @param source_led the source led instance to map to target
 * @param target_led a reference to the target who shall receive the mapping result
 * @exception ERROR_CODE_ARGUMENT_NULL if either @ref source_led or @target_led is NULL
 */
void map_rgb_ledc_led__to__rgb_led_diode(
    const struct ledc_rgb_led_t* source_led,
    rgb_led_diode_t* target_led
);



/**
 * @brief Maps the given led array into a domain bag of leds
 *  
 * @param source_leds an array of led instances to map to target.
 * @param source_leds_size the size of the led array to map. 
 * @param target_leds a reference to a bag of leds that shall receive the mapping results. This bag must be at least as big as @ref source_leds_size.
 * 
 * @exception ERROR_CODE_INSUFFICIENT_BUFFER if the @ref target_leds bag is too small to hold all the mappings of @ref source_leds.
 * @exception ERROR_CODE_ARGUMENT_NULL if either @ref source_leds, @ref target_leds is NULL or the bag points to a NULL led array.
 */
void map_rgb_ledc_led_array__to__rgb_led_diode_bag(
    const struct ledc_rgb_led_t * source_leds, uint8_t source_leds_size,
    rgb_led_diode_bag_t* target_leds
);