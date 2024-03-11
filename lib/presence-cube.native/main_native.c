#include "main_native.h"
#include "logger.h"
#include "get_led_query_handler.h"
#include "set_led_command_handler.h"
#include "rgb_ledc_adapter.h"
#include "exception_handling.h"
#include "stdlib.h"
#include "led_store.h"
#include "stdio.h"
#include "config.h"

#if CONFIG_WEBSERVER_ENABLED
#include "web_host.h"
#endif

// TODO: move this into a menuconfig
//       adding native menuconfig needs some hacking though
//       https://github.com/RobustoFramework/Multi-platform-Multi-board/tree/main/development
//       evaluate this solution or create a custom strategy

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
        rgb_led_diode_t led = response.led_bag->leds[i];
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
    initialize_modules();

    print_led_states();
    set_led_states();
    print_led_states();
}

int main()
{
#if CONFIG_WEBSERVER_ENABLED
    LOG_INFORMATION("-------------------------------------------------");
    LOG_INFORMATION("Running %s main %s build", "native", BUILD_ENVIRONMENT);
    LOG_INFORMATION("Running webserver on port " CONFIG_WEBSERVER_PORTS);
    LOG_INFORMATION("-------------------------------------------------");
    return web_host__host_web_application((const char *[]){
                                              "listening_ports",
                                              CONFIG_WEBSERVER_PORTS,
                                              "request_timeout_ms",
                                              CONFIG_WEBSERVER_REQUEST_TIMEOUT_MS,
                                              "error_log_file",
                                              CONFIG_WEBSERVER_ERROR_LOG_FILE,
                                              0},
                                          app_main, &cancellation_token);
#else
    LOG_INFORMATION("-------------------------------------------------");
    LOG_INFORMATION("Running %s main %s build", "native", BUILD_ENVIRONMENT);
    LOG_INFORMATION("-------------------------------------------------");
    return app_main(&cancellation_token);
#endif
}
