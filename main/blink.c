#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "wifi_setup.h"
#include "mqtt_setup.h"
#include "driver/ledc.h"
#include "zube_ledc.h"

#define LEDC_HS_CH0_GPIO_BLUE (18)
#define LEDC_HS_CH1_GPIO_RED (19)
#define LEDC_HS_CH2_GPIO_GREEN (17)

#define LEDC_HS_CH0_CHANNEL_BLUE LEDC_CHANNEL_0
#define LEDC_HS_CH1_CHANNEL_RED LEDC_CHANNEL_1
#define LEDC_HS_CH2_CHANNEL_GREEN LEDC_CHANNEL_2

#define LEDC_HS_TIMER LEDC_TIMER_0
#define LEDC_HS_MODE LEDC_HIGH_SPEED_MODE

#define RED_LED_PIN CONFIG_RGB_RED_CHANNEL_PIN
#define GREEN_LED_PIN CONFIG_RGB_GREEN_CHANNEL_PIN
#define YELLOW_LED_PIN CONFIG_RGB_BLUE_CHANNEL_PIN

static const char *TAG = "blink_main";

static ledc_channel_config_t ledc_channel[3] = {
    {.channel = LEDC_HS_CH0_CHANNEL_BLUE,
     .duty = 0,
     .gpio_num = LEDC_HS_CH0_GPIO_BLUE,
     .speed_mode = LEDC_HS_MODE,
     .hpoint = 0,
     .timer_sel = LEDC_HS_TIMER},
    {.channel = LEDC_HS_CH1_CHANNEL_RED,
     .duty = 0,
     .gpio_num = LEDC_HS_CH1_GPIO_RED,
     .speed_mode = LEDC_HS_MODE,
     .hpoint = 0,
     .timer_sel = LEDC_HS_TIMER},
    {.channel = LEDC_HS_CH2_CHANNEL_GREEN,
     .duty = 0,
     .gpio_num = LEDC_HS_CH2_GPIO_GREEN,
     .speed_mode = LEDC_HS_MODE,
     .hpoint = 0,
     .timer_sel = LEDC_HS_TIMER}};

/**
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/log.html#logging-library
 */
void configure_logging()
{
    // By default log only messages with levels <= warnings
    esp_log_level_set("*", ESP_LOG_WARN);
    // enable all log levels for the current module tag
    esp_log_level_set(TAG, ESP_LOG_VERBOSE);
}

void app_main(void)
{
    configure_logging();
    configure_ledc(ledc_channel);

    create_wifi_station();
    setup_mqtt_connection(ledc_channel);
}
