#pragma once
#include "led_domain_bag.h"

#ifdef PIO_UNIT_TESTING
    extern const led_domain_bag_t (*get_led_query_handler__led_register__get_leds)();
#endif

/**
 * @brief This function pointer acts as mediator between get_led_query_handler
 *        and the led_register.get_leds residing in a different bounded context.
 */
const led_domain_bag_t (*get_led_query_handler__led_register__get_leds)();