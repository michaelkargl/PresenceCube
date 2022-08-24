#include "get_led_query_handler.h"
#include "get_led_repository.h"
#include "logger.h"

static const char* _logger_context = "get_led_query_handler";


/**
 * @brief This function pointer acts as mediator between get_led_query_handler
 *        and the led_register.get_leds residing in a different bounded context.
 */
const rgb_led_domain_bag_t* (*get_led_query_handler__led_repository__get_leds)() = get_leds;

get_led_query_response_t handle_get_led_query(const get_led_query_t request)
{
    log_debug(_logger_context, "Handling get led query\n");

    return (get_led_query_response_t){
        // TODO: pass reference or by value?
        .led_bag = *get_led_query_handler__led_repository__get_leds()
    };
}