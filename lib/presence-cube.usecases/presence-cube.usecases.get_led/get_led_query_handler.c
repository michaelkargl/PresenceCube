#include "get_led_query_handler.h"
#include "get_led_repository.h"
#include "logger.h"

/**
 * @brief This function pointer acts as mediator between get_led_query_handler
 *        and the led_register.get_leds residing in a different bounded context.
 */
const rgb_led_domain_bag_t *(*get_led_query_handler__led_repository__get_leds)() = get_led_repository__get_leds;

const get_led_query_response_t get_led_query_handler__handle(const get_led_query_t request)
{
    LOG_DEBUG("Handling get led query...");
    return (get_led_query_response_t){
        .led_bag = get_led_query_handler__led_repository__get_leds()};
}