#include <stdlib.h>
#include "wifi_setup.h"
#include "freertos/task.h"
#include "display.h"
#include "esp_log.h"
#include "spiffs.h"
#include "mqtt_setup.h"
#include "zube_mqtt_handler.h"
#include "zube_ledc.h"

// configured using Kconfig.projbuild and idf menuconfig
// Mqtt connection
#define ESP_MQTT_URI CONFIG_ESP_MQTT_BROKER_URI
#define ESP_MQTT_BROKER_USER CONFIG_ESP_MQTT_BROKER_USER
#define ESP_MQTT_BROKER_PASSWORD CONFIG_ESP_MQTT_BROKER_PASSWORD
// Mqtt channels
// TODO: Add status/logging topic
#define ESP_MQTT_CONTROL_TOPIC CONFIG_ESP_MQTT_CONTROL_TOPIC
#define ESP_MQTT_DATA_TOPIC CONFIG_ESP_MQTT_DATA_TOPIC
// LED
#define RED_LED_PIN CONFIG_RGB_RED_CHANNEL_PIN
#define GREEN_LED_PIN CONFIG_RGB_GREEN_CHANNEL_PIN
#define BLUE_LED_PIN CONFIG_RGB_BLUE_CHANNEL_PIN


static const char *TAG = "main";
static esp_mqtt_client_handle_t _mqtt_client;
static ledc_channel_config_t _ledc_channels_rgb[3];
static esp_vfs_spiffs_conf_t spiffs_config = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 10,
    .format_if_mount_failed = true};

static ledc_timer_config_t _ledc_timer = {
    .duty_resolution = LEDC_TIMER_13_BIT,    // 13bit = 2^13 = 8192 levels within 1 cycle
    .freq_hz = 5000,                         // 5kHz = 1 cycle lasts 1/5000s
    .speed_mode = LEDC_LOW_SPEED_MODE,       // ESP32-S2 only supports configuring channels in “low speed” mode
    .timer_num = LEDC_TIMER_0,               // TODO: still unsure what this timer does and how to choose it
    .clk_cfg = LEDC_AUTO_CLK                 // Auto select the source clock
};

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

static void _setup_rgb_channel(ledc_channel_config_t *ledc_config, int gpio_pin, ledc_channel_t channel)
{
    ledc_config->channel = channel;
    ledc_config->duty = 0;
    ledc_config->gpio_num = gpio_pin;
    ledc_config->speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_config->hpoint = 0;
    ledc_config->timer_sel = LEDC_TIMER_0;
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
    _setup_rgb_channel(&_ledc_channels_rgb[0], RED_LED_PIN, LEDC_CHANNEL_0);
    _setup_rgb_channel(&_ledc_channels_rgb[1], GREEN_LED_PIN, LEDC_CHANNEL_1);
    _setup_rgb_channel(&_ledc_channels_rgb[2], BLUE_LED_PIN, LEDC_CHANNEL_2);
    configure_ledc(_ledc_channels_rgb, _ledc_timer);
   
    ESP_LOGI(TAG, "Setting up wifi connection...");
    create_wifi_station();

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
