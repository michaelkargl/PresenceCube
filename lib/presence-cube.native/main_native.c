#include "main_native.h"
#include "logger.h"
#include "get_led_query_handler.h"
#include "CException.h"
#include "stdlib.h"

static const char *_logger_context = "main.native";

static void _initialize_modules();
static void _deinitialize_modules();
static void _handle_uncaught_errors(error_code_t error);

static void _handle_uncaught_errors(error_code_t error)
{
    log_error(_logger_context, "Uncaught error received: %i\n", error);

    log_error(_logger_context, "Deinitializing resources...\n");
    _deinitialize_modules();

    log_error(_logger_context, "Exiting with status: %i\n", error);
    exit(error);
}

static void _initialize_modules()
{
    log_information(_logger_context, "Initializing modules...\n");
    uncaught_error_handler_init(_handle_uncaught_errors);
}

static void _deinitialize_modules()
{
    log_information(_logger_context, "Deinitializing modules...");
    uncaught_error_handler_deinit();
}

int main()
{
    log_information("main", "Running %s main %s build\n", "native", BUILD_ENVIRONMENT);

    _initialize_modules();

    get_led_query_response_t response = handle_get_led_query((get_led_query_t){});

    log_debug(_logger_context, "Queried a list of %i leds\n", response.led_bag.count);
    for (uint8_t i = 0; i < response.led_bag.count; i++)
    {
        rgb_led_domain_t led = response.led_bag.leds[i];
        log_debug(_logger_context, "  Led %i: %s\n", i, led.display_name);
    }

    return 0;
}
