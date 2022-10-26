#include "logger.h"
#include "spiffs.h"

#include "wifi_init.h"
#include "wifi_setup.h"

#include "led_store.h"

#include "delay_service.h"

#include "webserver.h"
#include "led_endpoints.h"
#include "hud_controller.h"

#include "get_led_query_handler.h"
#include "set_led_command_handler.h"
#include "rgb_ledc_adapter.h"

#include "exception_handling.h"

#define LED_FADE_MILLISECONDS CONFIG_PWM_FADE_INTERVAL
#define R_ON 100
#define G_ON 100
#define B_ON 100
#define R_OFF 0
#define G_OFF 0
#define B_OFF 0

// TODO: expose feature toggles as menuconfig entries
#define CUBE_WIFI_ENABLED 1
#define CUBE_WEBSERVER_ENABLED CUBE_WIFI_ENABLED & 1
#define CUBE_LED_API_ENABLED CUBE_WEBSERVER_ENABLED & 1
#define CUBE_HUD_API_ENABLED CUBE_WEBSERVER_ENABLED & 1
#define CUBE_STARTUP_LED_TEST 1

static const char *TAG = "main";

static esp_vfs_spiffs_conf_t spiffs_config = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 10,
    .format_if_mount_failed = true};

const rgb_led_domain_bag_t *_led_bag;

// prototypes
static void _initialize_modules();
static void _deinitialize_modules();
static void _handle_uncaught_errors(error_code_t error);

static void _handle_uncaught_errors(error_code_t error)
{
    log_error(TAG, "Uncaught error received: %i\n", error);

    log_error(TAG, "Deinitializing resources...\n");
    _deinitialize_modules();

    log_error(TAG, "Exiting with status: %i\n", error);
    exit(error);
}

static void _set_leds(
    const rgb_led_domain_bag_t *led_bag,
    uint8_t red_percent,
    uint8_t green_percent,
    uint8_t blue_percent)
{
    for (uint8_t i = 0; i < led_bag->count; i++)
    {
        rgb_led_domain_t led = led_bag->leds[i];
        set_led_command_handler__handle((set_led_command_t){
            .id = led.id,
            .red = red_percent,
            .green = green_percent,
            .blue = blue_percent});
    }
    _delay_ms(LED_FADE_MILLISECONDS);
}

static void _initialize_modules()
{
    log_information(TAG, "Initializing modules...\n");
    uncaught_error_handler_init(_handle_uncaught_errors);
    rgb_ledc_adapter__initialize();
}

static void _deinitialize_modules()
{
    log_information(TAG, "Deinitializing modules...\n");
    uncaught_error_handler_deinit();
}

int app_main()
{
    _initialize_modules();

    initialize_spiffs(&spiffs_config);

    log_information(TAG, "Setting up LED channels...\n");

    ESP_ERROR_CHECK(led_store__initialize());
    _led_bag = get_led_query_handler__handle((const get_led_query_t){}).led_bag;
    log_information(TAG, "%i LEDS are registered.\n", _led_bag->count);
    for (uint8_t i = 0; i < _led_bag->count; i++)
    {
        log_information(TAG, "LED %i: %s\n", _led_bag->leds[i].id, _led_bag->leds[i].display_name);
    }

#if CUBE_STARTUP_LED_TEST == 1
    const get_led_query_response_t response = get_led_query_handler__handle((get_led_query_t){});
    _set_leds(response.led_bag, R_ON, G_OFF, B_OFF);
    _set_leds(response.led_bag, R_OFF, G_ON, B_OFF);
    _set_leds(response.led_bag, R_OFF, G_OFF, B_ON);
#endif

#if CUBE_WIFI_ENABLED == 1
    log_information(TAG, "Setting up wifi connection...\n");
    create_wifi_station();
#endif

#if CUBE_WEBSERVER_ENABLED == 1
    log_information(TAG, "Setting up web server...\n");
    log_information(TAG, "Initializing controllers...\n");

    log_information(TAG, "Initializing / starting webserver...\n");
    webserver_start();
#endif

#if CUBE_LED_API_ENABLED == 1
    webserver_register_endpoints(get_led_controller_endpoints(), get_led_controller_endpoint_count());
#endif

#if CUBE_HUD_API_ENABLED == 1
    initialize_hud_controller();
    webserver_register_endpoints(get_hud_controller_endpoints(), get_hud_controller_endpoint_count());
#endif

    _set_leds(response.led_bag, R_OFF, G_OFF, G_OFF);
    log_information(TAG, "Setup done.\n");

    heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
    return 0;
}