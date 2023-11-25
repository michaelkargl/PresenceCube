#include "main_native.h"
#include "logger.h"
#include "get_led_query_handler.h"
#include "set_led_command_handler.h"
#include "rgb_ledc_adapter.h"
#include "exception_handling.h"
#include "stdlib.h"
#include "led_store.h"
#include "stdio.h"
#include "web_host.h"

#define REQUEST_TIMEOUT_MS "10000"
#define ERROR_LOG_FILE "error.log"
#define PORTS "8888,8884"

static bool cancellation_token = false;

static void initialize_modules();
static void deinitialize_modules();
static void handle_uncaught_errors(error_code_t error);

static void handle_uncaught_errors(error_code_t error)
{
    LOG_ERROR("Uncaught error received: %i", error);
    LOG_ERROR("Deinitializing resources...");
    deinitialize_modules();

    LOG_ERROR("Exiting with status: %i", error);
    exit(error);
}

static void initialize_modules()
{
    LOG_INFORMATION("Initializing modules...");
    uncaught_error_handler_init(handle_uncaught_errors);
    led_store__initialize();
    rgb_ledc_adapter__initialize();
}

static void deinitialize_modules()
{
    LOG_INFORMATION("Deinitializing modules...");
    uncaught_error_handler_deinit();
}

static void print_led_states()
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

static void set_led_states()
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

static int app_main(bool *cancellation_token)
{
    LOG_INFORMATION("-------------------------------------------------");
    LOG_INFORMATION("Running %s main %s build", "native", BUILD_ENVIRONMENT);
    LOG_INFORMATION("Running webserver on port "PORTS);
    LOG_INFORMATION("-------------------------------------------------");

    initialize_modules();

    print_led_states();
    set_led_states();
    print_led_states();
}

int main()
{
    return web_host__host_web_application((const char *[]){
                                    "listening_ports",
                                    PORTS,
                                    "request_timeout_ms",
                                    REQUEST_TIMEOUT_MS,
                                    "error_log_file",
                                    ERROR_LOG_FILE,
                                    0},
                                app_main, &cancellation_token);
}
