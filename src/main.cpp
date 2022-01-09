extern "C" {
    #include <stdlib.h>
    #include "spiffs/spiffs.h"
    #include "wifi/wifi_setup.h"
    #include "display.h"
    #include "esp_log.h"
    #include "led_control/led_control_init.h"
    #include "web/controllers/led_controller.h"
    #include "rgb_ledc.h"
    #include "delay_service.h"
    #include "webserver.h"
}

#include <gdew027w3T.h>

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

static FT6X36 touch_panel(CONFIG_TOUCH_INT);
static EpdSpi spi_io_interface;
static Gdew027w3T display(spi_io_interface, touch_panel);
static const bool cleanScreenAtStart = true;

static void _draw_text(char* text) {
    uint8_t font_size_x = 5;

    display.setCursor(10, 10);
    display.setTextColor(EPD_WHITE);
    display.setTextSize(font_size_x);
    display.setTextWrap(true);
    display.println(text);
}

static void _draw_circles(int interval, bool autoflush = false) {
    ESP_LOGI(TAG, "Drawing circles...");
    const int16_t center_x = GDEW027W3_WIDTH / 2;
    const int16_t center_y = GDEW027W3_HEIGHT /2;

    for (int16_t radius = GDEW027W3_HEIGHT; radius > 0; radius -= interval) {
        ESP_LOGD(TAG, "   circle (x%i,y%i)-> r%i", center_x, center_y, radius);
        display.drawCircle(center_x, center_y, radius, EPD_BLACK);
    }

    if (autoflush) {
        ESP_LOGD(TAG, "   Flushing circles to display");
        display.update();
    }
}



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

extern "C" void app_main()
{    
    // set all components to ERROR level
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    ESP_LOGI(TAG, "Setting up spiffs...");
    initialize_spiffs(&spiffs_config);

    // TODO: Move into separate initializer module
    ESP_LOGI(TAG, "Initializing E-Ink display...");
    display.init(false);

    if (cleanScreenAtStart) {
        ESP_LOGD(TAG, "Clearing screen...");
        display.update();
    }
    // TODO: Move into separate service
    _draw_circles(2);
    _draw_text("Hello :)");
    display.update();

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
    initialize_led_controller(_leds, _leds_count);

    ESP_LOGI(TAG, "Initializing / starting webserver...");
    ESP_ERROR_CHECK(start_webserver());
    ESP_ERROR_CHECK(register_endpoint(get_led_controller_endpoints()->get_hello));
    ESP_ERROR_CHECK(register_endpoint(get_led_controller_endpoints()->get_leds));

    // _mqtt_client = _connect_mqtt();

    ESP_LOGI(TAG, "Setup done.");

    set_leds_color_percent(_leds, _leds_count, 0, 0, 0);
}