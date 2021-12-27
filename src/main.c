#include <stdlib.h>
#include "wifi/spiffs.h"
#include "wifi/wifi_setup.h"
#include "display.h"
#include "esp_log.h"
#include "led_control/led_control_init.h"
#include "web/controllers/led_controller.h"
#include "rgb_ledc.h"
#include "delay_service.h"
#include "webserver.h"

// configured using Kconfig.projbuild and idf menuconfig
// Mqtt connection
#define ESP_MQTT_URI CONFIG_ESP_MQTT_BROKER_URI
#define ESP_MQTT_BROKER_USER CONFIG_ESP_MQTT_BROKER_USER
#define ESP_MQTT_BROKER_PASSWORD CONFIG_ESP_MQTT_BROKER_PASSWORD
// Mqtt channels
// TODO: Add status/logging topic
#define ESP_MQTT_CONTROL_TOPIC CONFIG_ESP_MQTT_CONTROL_TOPIC
#define ESP_MQTT_DATA_TOPIC CONFIG_ESP_MQTT_DATA_TOPIC


static const char *TAG = "main";
static esp_vfs_spiffs_conf_t spiffs_config = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 10,
    .format_if_mount_failed = true};

// static esp_mqtt_client_handle_t _mqtt_client;
static const struct ledc_rgb_led_t* _leds;
static int _leds_count = 0;

// static esp_mqtt_client_handle_t _connect_mqtt()
// {
//     ESP_LOGI(TAG, "Setting up MQTT connection...");

//     esp_mqtt_client_handle_t mqtt_client = setup_mqtt_connection(
//         ESP_MQTT_URI,
//         ESP_MQTT_BROKER_USER,
//         ESP_MQTT_BROKER_PASSWORD);

//     mqtt_handlers_t handlers = initialize_handlers(
//         mqtt_client,
//         ESP_MQTT_CONTROL_TOPIC,
//         ESP_MQTT_DATA_TOPIC);

//     register_mqtt_event_handlers(handlers);

//     return mqtt_client;
// }



void app_main()
{    
    // set all components to ERROR level
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    ESP_LOGI(TAG, "Setting up spiffs...");
    initialize_spiffs(&spiffs_config);

    // ESP_LOGI(TAG, "Setting up display with fonts...");
    // const char *font_gothic_16x32 = "/spiffs/ILGH32XB.FNT";
    // initialize_display(
    //     CONFIG_MOSI_GPIO,
    //     CONFIG_SCLK_GPIO,
    //     CONFIG_CS_GPIO,
    //     CONFIG_DC_GPIO,
    //     CONFIG_RESET_GPIO,
    //     CONFIG_BL_GPIO,
    //     CONFIG_WIDTH,
    //     CONFIG_HEIGHT,
    //     CONFIG_OFFSETX,
    //     CONFIG_OFFSETY,
    //     font_gothic_16x32);

    // Prepare colorful backdrop
    // for(int i = 0; i < 700; i++) {
    //     display_random_rectangle();
    // }

    // TODO: Review feature
    ESP_LOGI(TAG, "Setting up LED channels...");
    ESP_ERROR_CHECK(initialize_led_control());
    _leds = get_led_control_leds();
    _leds_count = get_led_control_initialized_led_count();

    set_leds_color_percent(_leds, _leds_count, 100, 0, 0);
    _delay_ms(1000);
    set_leds_color_percent(_leds, _leds_count, 0, 0, 100);
    _delay_ms(1000);
    set_leds_color_percent(_leds, _leds_count, 0, 100, 0);
    _delay_ms(1000);
    
    ESP_LOGI(TAG, "Setting up wifi connection...");
    create_wifi_station();

    ESP_LOGI(TAG, "Setting up web server...");
    ESP_LOGI(TAG, "Initializing controllers...");
    initialize_led_controller(&_leds[0]);

    ESP_LOGI(TAG, "Initializing / starting webserver...");
    ESP_ERROR_CHECK(start_webserver());
    ESP_ERROR_CHECK(register_endpoint(get_led_controller_endpoints()->get_hello));
    ESP_ERROR_CHECK(register_endpoint(get_led_controller_endpoints()->get_leds));

    // _mqtt_client = _connect_mqtt();

    ESP_LOGI(TAG, "Setup done.");

    set_leds_color_percent(_leds, _leds_count, 0, 0, 0);
}
