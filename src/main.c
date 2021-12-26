#include <stdlib.h>
#include "wifi_setup.h"
#include "freertos/task.h"
#include "display.h"
#include "esp_log.h"
#include "spiffs.h"
#include "mqtt_setup.h"
#include "zube_mqtt_handler.h"
#include "web/webserver_init.h"
#include "led_control/led_control_init.h"
#include "rgb_ledc.h"

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
static esp_mqtt_client_handle_t _mqtt_client;
static esp_vfs_spiffs_conf_t spiffs_config = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 10,
    .format_if_mount_failed = true};

static const struct ledc_rgb_led_t* _leds;
static int _leds_count = 0;


static void _delay_ms(int milliseconds) { 
    vTaskDelay(milliseconds / portTICK_PERIOD_MS); 
}

static esp_mqtt_client_handle_t _connect_mqtt()
{
    ESP_LOGI(TAG, "Setting up MQTT connection...");

    esp_mqtt_client_handle_t mqtt_client = setup_mqtt_connection(
        ESP_MQTT_URI,
        ESP_MQTT_BROKER_USER,
        ESP_MQTT_BROKER_PASSWORD);

    mqtt_handlers_t handlers = initialize_handlers(
        mqtt_client,
        ESP_MQTT_CONTROL_TOPIC,
        ESP_MQTT_DATA_TOPIC);

    register_mqtt_event_handlers(handlers);

    return mqtt_client;
}


void app_main()
{
    // set all components to ERROR level
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    ESP_LOGI(TAG, "Setting up spiffs...");
    initialize_spiffs(&spiffs_config);

    ESP_LOGI(TAG, "Setting up display with fonts...");
    const char *font_gothic_16x32 = "/spiffs/ILGH32XB.FNT";
    initialize_display(
        CONFIG_MOSI_GPIO,
        CONFIG_SCLK_GPIO,
        CONFIG_CS_GPIO,
        CONFIG_DC_GPIO,
        CONFIG_RESET_GPIO,
        CONFIG_BL_GPIO,
        CONFIG_WIDTH,
        CONFIG_HEIGHT,
        CONFIG_OFFSETX,
        CONFIG_OFFSETY,
        font_gothic_16x32);

    // Prepare colorful backdrop
    for(int i = 0; i < 700; i++) {
        display_random_rectangle();
    }

    // TODO: Review feature
    ESP_LOGI(TAG, "Setting up LED channels...");
    ESP_ERROR_CHECK(initialize_led_control());
    _leds = get_led_control_leds();
    _leds_count = get_led_control_initialized_led_count();

    ESP_LOGI(TAG, "Set up %i LEDs...", _leds_count);
    for ( int i = 0; i < _leds_count; i++ ) {
        const struct ledc_rgb_led_t led = _leds[i];
        if (led.is_initialized) {
            set_led_color_percent(&led, 100, 0, 0);
            _delay_ms(led.fade_milliseconds);
            set_led_color_percent(&led, 0, 100, 0); 
            _delay_ms(led.fade_milliseconds);
            set_led_color_percent(&led, 0, 0, 100); 
            _delay_ms(led.fade_milliseconds);
        }
    }
    

    ESP_LOGI(TAG, "Setting up wifi connection...");
    create_wifi_station();

    ESP_LOGI(TAG, "Setting up web server...");
    ESP_ERROR_CHECK(initialize_webserver());

    _mqtt_client = _connect_mqtt();

    ESP_LOGI(TAG, "Setup done.");


    // Display counter
    // uint8_t counter_string[6];
    // uint16_t loop_counter = 0;
    // rgb565_t black = get_rgb565color(0, 0, 0);
    // rgb565_t white = get_rgb565color(255, 255, 255);
    // while (1)
    // {
    //     loop_counter++;

    //     // display_random_rectangle();

    //     sprintf((char *)counter_string, "%i", loop_counter);
    //     draw_string(
    //         10, 20, 
    //         black,
    //         white, 
    //         counter_string);    

    //     set_led_color_percent(
    //         _ledc_channels_rgb,
    //         loop_counter % 2 == 0 ? 100 : 0,
    //         loop_counter % 3 == 0 ? 100 : 0,
    //         loop_counter % 5 == 0 ? 100 : 0,
    //         0);

    //     // delay task to not starve sister threads (wifi)
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
}
