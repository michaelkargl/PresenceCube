#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "wifi_setup.h"
#include "mqtt_setup.h"
#include "driver/ledc.h"
#include "zube_ledc.h"

// configured using Kconfig.projbuild and idf menuconfig
#define LEDC_TRANSITION_INTERVAL CONFIG_PULSE_WIDTH_MODULATION_FADE_INTERVAL
#define LEDC_HS_CH0_CHANNEL_BLUE LEDC_CHANNEL_0
#define LEDC_HS_CH1_CHANNEL_RED LEDC_CHANNEL_1
#define LEDC_HS_CH2_CHANNEL_GREEN LEDC_CHANNEL_2

#define LEDC_HS_TIMER LEDC_TIMER_0
#define LEDC_HS_MODE LEDC_HIGH_SPEED_MODE

#define RED_LED_PIN CONFIG_RGB_RED_CHANNEL_PIN
#define GREEN_LED_PIN CONFIG_RGB_GREEN_CHANNEL_PIN
#define BLUE_LED_PIN CONFIG_RGB_BLUE_CHANNEL_PIN

static const char *TAG = "blink_main";

static ledc_channel_config_t ledc_channel[3] = {
    {.channel = LEDC_HS_CH0_CHANNEL_BLUE,
     .duty = 0,
     .gpio_num = BLUE_LED_PIN,
     .speed_mode = LEDC_HS_MODE,
     .hpoint = 0,
     .timer_sel = LEDC_HS_TIMER},
    {.channel = LEDC_HS_CH1_CHANNEL_RED,
     .duty = 0,
     .gpio_num = RED_LED_PIN,
     .speed_mode = LEDC_HS_MODE,
     .hpoint = 0,
     .timer_sel = LEDC_HS_TIMER},
    {.channel = LEDC_HS_CH2_CHANNEL_GREEN,
     .duty = 0,
     .gpio_num = GREEN_LED_PIN,
     .speed_mode = LEDC_HS_MODE,
     .hpoint = 0,
     .timer_sel = LEDC_HS_TIMER}};

void app_main(void)
{
    create_wifi_station();
    
    configure_ledc(ledc_channel);
    setup_mqtt_connection(ledc_channel);
}
