extern "C" {
    #include <stdlib.h>
    #include "spiffs/spiffs.h"
    #include "wifi/wifi_setup.h"
    #include "esp_log.h"
    #include "led_control/led_control_init.h"
    #include "web/controllers/led_controller.h"
    #include "rgb_ledc.h"
    #include "delay_service.h"
    #include "webserver.h"
}

#include "display/display.hpp"
#include "display/display_init.h"
#include "display/heads_up_display.hpp"
#include "gdew_colors.h"
#include "web/controllers/web_controller_registrar.hpp"
#include "web/controllers/hud_controller.hpp"
#include "string.h"


static const char *TAG = "main";
static esp_vfs_spiffs_conf_t spiffs_config = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 10,
    .format_if_mount_failed = true};

static const struct ledc_rgb_led_t* _leds;
static int _leds_count = 0;
static HeadsUpDisplay _heads_up_display = NULL;


extern "C" void app_main()
{    
    // set all components to ERROR level
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    ESP_LOGI(TAG, "Setting up spiffs...");
    initialize_spiffs(&spiffs_config);

    // TODO: Move into separate initializer module
    ESP_LOGI(TAG, "Initializing E-Ink display...");
    new(&_heads_up_display) HeadsUpDisplay(initialize_display());

    _heads_up_display.drawBackground();
    const char* center_label = "Employees";
    _heads_up_display.updateCenterLabel(center_label, strlen(center_label));
    
    const char* center_value = "AB";
   _heads_up_display.updateCenterValue(center_value, strlen(center_value));

    const char* bottom_text = "Updated: 13:43";
    _heads_up_display.updateBottomText(bottom_text, strlen(bottom_text));

    const char* top_text = "Runlevel: 3";
    _heads_up_display.updateTopText(top_text, strlen(top_text));
    _heads_up_display.flushUpdates();

    ESP_LOGI(TAG, "Setting up LED channels...");
    ESP_ERROR_CHECK(initialize_led_control());
    _leds = get_led_control_leds();
    _leds_count = get_led_control_initialized_led_count();

    // set_leds_color_percent(_leds, _leds_count, 100, 0, 0);
    // _delay_ms(1000);
    // set_leds_color_percent(_leds, _leds_count, 0, 0, 100);
    // _delay_ms(1000);
    // set_leds_color_percent(_leds, _leds_count, 0, 100, 0);
    // _delay_ms(1000);
    
    ESP_LOGI(TAG, "Setting up wifi connection...");
    create_wifi_station();

    ESP_LOGI(TAG, "Setting up web server...");
    ESP_LOGI(TAG, "Initializing controllers...");
    initialize_led_controller(_leds, _leds_count);

    ESP_LOGI(TAG, "Initializing / starting webserver...");

    httpd_handle_t web_server = start_webserver();
    WebControllerRegistrar registrar(web_server);
    registrar.registerController(new HudController(&_heads_up_display));
    
    uint32_t endpint_count = 0;
    registrar.registerEndpoints(get_led_controller_endpoints(&endpint_count), endpint_count);


    ESP_LOGI(TAG, "Setup done.");

    set_leds_color_percent(_leds, _leds_count, 0, 0, 0);
}