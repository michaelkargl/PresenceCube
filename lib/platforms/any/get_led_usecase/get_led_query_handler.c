#include "get_led_query_handler.h"
#include "led_register.h"
#include "logger.h"
#include "errno.h"
#include "string.h"
#include <stdlib.h>

static const char* _logger_context = "get_led_query_handler";

/**
 * @brief This function pointer acts as mediator between get_led_query_handler
 *        and the led_register.get_leds residing in a different bounded context.
 */
const led_domain_bag_t (*get_led_query_handler__led_register__get_leds)() = get_leds;


const get_led_query_response_t handle_get_led_query(const get_led_query_t request)
{
    log_debug(_logger_context, "Handling get led query\n");

    const led_domain_bag_t bag = get_led_query_handler__led_register__get_leds();
    return (get_led_query_response_t){
        .led_bag = bag};
}