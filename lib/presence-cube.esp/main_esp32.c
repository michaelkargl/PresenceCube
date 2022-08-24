#include "logger.h"
#include "spiffs.h"

#include "wifi_init.h"
#include "wifi_setup.h"

#include "rgb_ledc_init.h"
#include "rgb_ledc.h"

#include "led_store.h"
#include "get_led_repository.h"

#include "delay_service.h"

#include "webserver.h"
#include "led_controller.h"
#include "hud_controller.h"

#include "get_led_query_handler.h"
#include "CException.h"


#define LED_FADE_MILLISECONDS 4000
#define R_ON 100
#define G_ON 100
#define B_ON 100
#define R_OFF 0
#define G_OFF 0
#define B_OFF 0

static const char *TAG = "main";

static esp_vfs_spiffs_conf_t spiffs_config = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 10,
    .format_if_mount_failed = true};


static const rgb_led_domain_bag_t* _led_bag;
static const struct ledc_rgb_led_t *_leds;
static int _leds_count = 0;


// prototypes
static void _initialize_modules();
static void _deinitialize_modules();
static void _handle_uncaught_errors(error_code_t error);


static void _handle_uncaught_errors(error_code_t error) {
    log_error(TAG, "Uncaught error received: %i\n", error);
    
    log_error(TAG, "Deinitializing resources...\n");
    _deinitialize_modules();
    
    log_error(TAG, "Exiting with status: %i\n", error);
    exit(error);
}

static void _initialize_modules() {
    log_information(TAG, "Initializing modules...\n");
    uncaught_error_handler_init(_handle_uncaught_errors);
}

static void _deinitialize_modules() {
    log_information(TAG, "Deinitializing modules...\n");
    uncaught_error_handler_deinit();
}

int app_main()
{
    _initialize_modules();

    initialize_spiffs(&spiffs_config);

    log_information(TAG, "Setting up LED channels...\n");

    ESP_ERROR_CHECK(led_store_initialize());
    _led_bag = get_leds();
    log_information(TAG, "%i LEDS are registered.\n", _led_bag->count);
    for(uint8_t i = 0; i < _led_bag->count; i++) {
        log_information(TAG, "LED %i: %s\n", _led_bag->leds[i].id, _led_bag->leds[i].display_name);
    }

    _leds = led_store__get_leds();
    _leds_count = led_store__get_led_count();
    configure_rgb_leds(_leds, _leds_count);
    log_information(TAG, "%i LEDS are registered.\n", _leds_count);

    set_leds_color_percent(_leds, _leds_count, R_ON, G_OFF, B_OFF, LED_FADE_MILLISECONDS);
    _delay_ms(LED_FADE_MILLISECONDS);
    set_leds_color_percent(_leds, _leds_count, R_OFF, G_OFF, B_ON, LED_FADE_MILLISECONDS);
    _delay_ms(LED_FADE_MILLISECONDS);
    set_leds_color_percent(_leds, _leds_count, R_OFF, G_ON, B_OFF, LED_FADE_MILLISECONDS);
    _delay_ms(LED_FADE_MILLISECONDS);

    log_information(TAG, "Setting up wifi connection...\n");
    create_wifi_station();

    log_information(TAG, "Setting up web server...\n");
    log_information(TAG, "Initializing controllers...\n");
    initialize_led_controller(_leds, _leds_count);
    initialize_hud_controller();

    log_information(TAG, "Initializing / starting webserver...\n");
    webserver_start();
    webserver_register_endpoints(get_led_controller_endpoints(), get_led_controller_endpoint_count());
    webserver_register_endpoints(get_hud_controller_endpoints(), get_hud_controller_endpoint_count());

    log_information(TAG, "Setup done.\n");
    set_leds_color_percent(_leds, _leds_count, 0, 0, 0, LED_FADE_MILLISECONDS);

    heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
    return 0;
}