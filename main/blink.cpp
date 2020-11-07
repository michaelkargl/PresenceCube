#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "number_display.h"
#include "esp_log.h"

extern "C"
{
#include "zube_ledc.h"
#include "wifi_setup.h"
#include "mqtt_setup.h"
}

// configured using Kconfig.projbuild and idf menuconfig
#define LEDC_TRANSITION_INTERVAL CONFIG_PULSE_WIDTH_MODULATION_FADE_INTERVAL
#define LEDC_HIGH_SPEED_TIMER LEDC_TIMER_0
#define RED_LED_PIN CONFIG_RGB_RED_CHANNEL_PIN
#define GREEN_LED_PIN CONFIG_RGB_GREEN_CHANNEL_PIN
#define BLUE_LED_PIN CONFIG_RGB_BLUE_CHANNEL_PIN

#define LEDC_TRANSITION_INTERVAL CONFIG_PULSE_WIDTH_MODULATION_FADE_INTERVAL

typedef struct
{
    int red;
    int green;
    int blue;
} led_pins;

typedef struct
{
    ledc_channel_t red;
    ledc_channel_t green;
    ledc_channel_t blue;
} led_channels;

NumberDisplay tft_display_service;
ledc_channel_config_t ledc_channel[3];
const char *TAG = "blink.cpp";

void setup_rgb_channel(ledc_channel_config_t *ledc_config, int gpio_pin, ledc_channel_t channel)
{
    ledc_config->channel = channel;
    ledc_config->duty = 0;
    ledc_config->gpio_num = gpio_pin;
    ledc_config->speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_config->hpoint = 0;
    ledc_config->timer_sel = LEDC_HIGH_SPEED_TIMER;
}

void setup_rgb_channels(ledc_channel_config_t *ledc_config, led_pins *gpio_pins, led_channels *channels)
{
    setup_rgb_channel(&ledc_config[0], gpio_pins->red, channels->red);
    setup_rgb_channel(&ledc_config[1], gpio_pins->green, channels->green);
    setup_rgb_channel(&ledc_config[2], gpio_pins->blue, channels->blue);
}

void draw_screen(NumberDisplay *display, int number)
{
    auto random = rand() % 100;
    auto filledEllipses = random % 2 == 1;

    display->clearScreen();
    display->displayEllipses(filledEllipses);
    display->displayNumber(number);

    // delay(2000);
}

void handle_led_message(cJSON *senmlRecord)
{
    //TODO: Error handling
    char *rgbValueJson = cJSON_GetObjectItem(senmlRecord, "vs")->valuestring;
    cJSON *rgbValues = cJSON_Parse(rgbValueJson);

    int r = max(cJSON_GetObjectItem(rgbValues, "r")->valueint, 255);
    int g = max(cJSON_GetObjectItem(rgbValues, "g")->valueint, 255);
    int b = max(cJSON_GetObjectItem(rgbValues, "b")->valueint, 255);
    ESP_LOGD(TAG, "Received request to set led-color to %i,%i,%i", r, g, b);

    float r_percent = floor((100 / (float)255) * r);
    float g_percent = floor((100 / (float)255) * g);
    float b_percent = floor((100 / (float)255) * b);

    cJSON_Delete(rgbValues); // dispose

    ESP_LOGD(TAG, "Received request to set led-color to %f,%f,%f", r_percent, g_percent, b_percent);
    set_led_color_percent(
        ledc_channel,
        r_percent,
        g_percent,
        b_percent,
        LEDC_TRANSITION_INTERVAL);
}

void handle_number_message(cJSON *senml_record)
{
    cJSON *jsonItem = cJSON_GetObjectItem(senml_record, "n");
    if (cJSON_IsNumber(jsonItem))
    {
        int number = jsonItem->valueint;
        ESP_LOGD(TAG, "Received Number: %i", number);

        draw_screen(&tft_display_service, number);
    }
    else
    {
        ESP_LOGE(TAG, "Received value: %s was not a number.", senml_record->valuestring);
    }
}

void handle_mqtt_mesasge(char *message)
{
    ESP_LOGI(TAG, "=======================================");
    ESP_LOGI(TAG, "Received message: %s", message);
    ESP_LOGI(TAG, "=======================================");

    cJSON *root = cJSON_Parse(message);
    int packSize = cJSON_GetArraySize(root);

    for (int i = 0; i < packSize; i++)
    {
        cJSON *senmlRecord = cJSON_GetArrayItem(root, i);
        char *actuator_name = cJSON_GetObjectItem(senmlRecord, "bn")->valuestring;

        if (strcmp(actuator_name, "led_rgb") == 0)
        {
            handle_led_message(senmlRecord);
        }
        else if (strcmp(actuator_name, "number") == 0)
        {
            handle_number_message(senmlRecord);
        }

        // free memory
        cJSON_Delete(senmlRecord);
        senmlRecord = 0;
    }
}

extern "C" void app_main(void)
{
    led_pins pins = {
        RED_LED_PIN,
        GREEN_LED_PIN,
        BLUE_LED_PIN};
    led_channels channels = {
        LEDC_CHANNEL_1,
        LEDC_CHANNEL_2,
        LEDC_CHANNEL_0};

    draw_screen(&tft_display_service, 0);

    setup_rgb_channels(ledc_channel, &pins, &channels);
    create_wifi_station();

    configure_ledc(ledc_channel);
    setup_mqtt_connection(handle_mqtt_mesasge);
}
