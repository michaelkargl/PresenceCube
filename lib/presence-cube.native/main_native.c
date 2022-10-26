#include "main_native.h"
#include "logger.h"
#include "get_led_query_handler.h"
#include "set_led_command_handler.h"
#include "exception_handling.h"
#include "stdlib.h"
#include "led_store.h"
#include "stdio.h"

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
    led_store__initialize();
}

static void _deinitialize_modules()
{
    log_information(_logger_context, "Deinitializing modules...");
    uncaught_error_handler_deinit();
}

static void _print_led_states()
{
    const get_led_query_response_t response = get_led_query_handler__handle((get_led_query_t){});

    LOG_DEBUG("Queried a list of %i leds\n", response.led_bag->count);
    for (uint8_t i = 0; i < response.led_bag->count; i++)
    {
        rgb_led_domain_t led = response.led_bag->leds[i];
        LOG_DEBUG(
            "  Led %u %s has color (%u, %u, %u)%%.",
            led.id,
            led.display_name,
            led.red.brightness_percent,
            led.green.brightness_percent,
            led.blue.brightness_percent);
    }
}

static void _set_led_states()
{
    const get_led_query_response_t response = get_led_query_handler__handle((get_led_query_t){});

    LOG_DEBUG("Queried a list of %i leds\n", response.led_bag->count);
    for (uint8_t i = 0; i < response.led_bag->count; i++)
    {
        set_led_command_handler__handle((set_led_command_t){
            .id = response.led_bag->leds[i].id,
            .red = i,
            .green = i,
            .blue = i});
    }
}

int main()
{
    log_information("main", "Running %s main %s build\n", "native", BUILD_ENVIRONMENT);

    _initialize_modules();

    _print_led_states();
    _set_led_states();
    _print_led_states();

    return 0;
}
