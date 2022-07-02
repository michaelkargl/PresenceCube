#include "get_led_query_handler.h"
#include "get_led_query_handler_globals.h"
#include "led_register.h"
#include "logger.h"
#include "errno.h"
#include "string.h"
#include <stdlib.h>

static const char* _logger_context = "get_led_query_handler";

static void _ensure_module_initialized()
{
    log_debug(_logger_context, "Ensuring module is initialized...\n");
    if (!get_led_query_handler_initialized())
    {
        errno = EPERM;
        log_error(_logger_context, "Ensure the module is initialized before use. "
                               "Exiting with errno (%i): %s\n",
                               errno, strerror(errno));
        exit(errno);
    }
}

void get_led_query_handler_init()
{
    log_debug(_logger_context, "Initializing module...\n");
    get_led_query_handler__led_register__get_leds = get_leds;
}

void get_led_query_handler_deinit()
{
    get_led_query_handler__led_register__get_leds = 0;
}

bool get_led_query_handler_initialized()
{
    return get_led_query_handler__led_register__get_leds != 0;
}

const get_led_query_response_t handle_get_led_query(const get_led_query_t request)
{
    _ensure_module_initialized();

    log_debug(_logger_context, "Handling get led query\n");

    const led_domain_bag_t bag = get_led_query_handler__led_register__get_leds();
    return (get_led_query_response_t){
        .led_bag = bag};
}