#include "logger.h"
#include "spiffs.h"

#include "wifi_init.h"
#include "wifi_setup.h"

#include "led_control_init.h"
#include "rgb_ledc.h"

#include "delay_service.h"

#include "webserver.h"
#include "led_controller.h"
#include "hud_controller.h"

#include "get_led_query_handler.h"


static const char *TAG = "main";

static esp_vfs_spiffs_conf_t spiffs_config = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 10,
    .format_if_mount_failed = true};


static const struct ledc_rgb_led_t *_leds;
static int _leds_count = 0;


void initialize_modules() {
    get_led_query_handler_init();
}

int app_main()
{

    initialize_spiffs(&spiffs_config);

    log_information(TAG, "Setting up LED channels...\n");
    
    ESP_ERROR_CHECK(initialize_led_control());
    _leds = get_led_control_leds();
    _leds_count = get_led_control_initialized_led_count();
    log_information(TAG, "%i LEDS are registered.\n", _leds_count);

    set_leds_color_percent(_leds, _leds_count, 100, 0, 0);
    _delay_ms(1000);
    set_leds_color_percent(_leds, _leds_count, 0, 0, 100);
    _delay_ms(1000);
    set_leds_color_percent(_leds, _leds_count, 0, 100, 0);
    _delay_ms(1000);

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
    set_leds_color_percent(_leds, _leds_count, 0, 0, 0);

    heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
    return 0;
}