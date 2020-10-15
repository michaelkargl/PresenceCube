#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "cJSON.h"
#include "string.h"
#include "driver/ledc.h"
#include <math.h>
#include "math_helper.h"
#include "wifi_setup.h"
#include "mqtt_setup.h"

//TODO: Extract MQTT into separate file => Provide separate events for each MQTT event
//TODO: Extract LEDC into separate file

// configured using Kconfig.projbuild and idf menuconfig
#define RED_LED_PIN CONFIG_RGB_RED_CHANNEL_PIN
#define GREEN_LED_PIN CONFIG_RGB_GREEN_CHANNEL_PIN
#define YELLOW_LED_PIN CONFIG_RGB_BLUE_CHANNEL_PIN


#define LEDC_HS_TIMER LEDC_TIMER_0
#define LEDC_HS_MODE LEDC_HIGH_SPEED_MODE
#define LEDC_HS_DUTY_RESOLUTION LEDC_TIMER_13_BIT
#define LEDC_TRANSITION_INTERVAL CONFIG_PULSE_WIDTH_MODULATION_FADE_INTERVAL

#define LEDC_HS_CH0_GPIO_BLUE (18)
#define LEDC_HS_CH1_GPIO_RED (19)
#define LEDC_HS_CH2_GPIO_GREEN (17)

#define LEDC_HS_CH0_CHANNEL_BLUE LEDC_CHANNEL_0
#define LEDC_HS_CH1_CHANNEL_RED LEDC_CHANNEL_1
#define LEDC_HS_CH2_CHANNEL_GREEN LEDC_CHANNEL_2

static const char *TAG = "toggle_led";

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


void set_color_hard(ledc_channel_config_t channel, int32_t duty)
{
    if(duty < 0)
        return;

    // configure channel updates
    ESP_LOGD(TAG, "Switching duty cycle of channel %d from %d => %d", channel.channel, channel.duty, duty);
    ledc_set_duty(
        channel.speed_mode,
        channel.channel,
        duty);

    // activate changes
    ledc_update_duty(channel.speed_mode, channel.channel);
}

void set_color_soft(ledc_channel_config_t channel, int32_t duty, int32_t fade_time_ms)
{
    if (duty < 0)
        return;

    ESP_LOGD(TAG, "Softly setting duty cycle of channel %d from %d => %d over %d ms",
             channel.channel, channel.duty, duty, fade_time_ms);
    ledc_set_fade_with_time(channel.speed_mode, channel.channel, duty, fade_time_ms);
    ledc_fade_start(channel.speed_mode, channel.channel, LEDC_FADE_NO_WAIT);
}

void set_led_color(ledc_channel_config_t *rgb_channels, int32_t duty_red, int32_t duty_green, int32_t duty_blue, int32_t fade_time_ms)
{
    ledc_channel_config_t channel_red = rgb_channels[0];
    ledc_channel_config_t channel_green = rgb_channels[1];
    ledc_channel_config_t channel_blue = rgb_channels[2];

    if (fade_time_ms > 0)
    {
        set_color_soft(channel_red, duty_red, fade_time_ms);
        set_color_soft(channel_green, duty_green, fade_time_ms);
        set_color_soft(channel_blue, duty_blue, fade_time_ms);
    }
    else
    {
        set_color_hard(channel_red, duty_red);
        set_color_hard(channel_green, duty_green);
        set_color_hard(channel_blue, duty_blue);
    }
}

int get_max_duty(int duty_resolution)
{
    int duty = pow(2, LEDC_HS_DUTY_RESOLUTION) - 1;

    ESP_LOGD(TAG, "Calculated max duty of %d with a resolution of %d", duty, duty_resolution);
    return duty;
}

int calculate_duty(int8_t percent)
{
    int max_duty = get_max_duty(LEDC_HS_DUTY_RESOLUTION);
    int duty = floor((max_duty / (float)100) * percent);
    duty = ranged_value(duty, 0, max_duty);

    ESP_LOGD(TAG, "Calculated duty %d for percent %d", duty, percent);
    return duty;
}

void set_led_color_percent(ledc_channel_config_t *channels, int percent_red, int percent_green, int percent_blue, int transition_interval_ms)
{
    percent_red = min(percent_red, 0);
    percent_green = min(percent_green, 0);
    percent_blue = min(percent_blue, 0);

    int duty_red   = calculate_duty(percent_red);
    int duty_green = calculate_duty(percent_green);
    int duty_blue  = calculate_duty(percent_blue);

    set_led_color(
        channels,
        duty_red,
        duty_green,
        duty_blue,
        transition_interval_ms);
}


void configure_ledc()
{
    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_HS_DUTY_RESOLUTION, // 13bit = 2^13 = 8192 levels within 1 cycle
        .freq_hz = 5000,                            // 5kHz = 1 cycle lasts 1/5000s
        .speed_mode = LEDC_HS_MODE,                 // high speed mode that supports fading change
        .timer_num = LEDC_HS_TIMER,                 // TODO: still unsure what this timer does and how to choose it
        .clk_cfg = LEDC_AUTO_CLK                    // Auto select the source clock
    };

    ledc_timer_config(&ledc_timer);

    //TODO: Figure out a way to configure the ledc channels within a function => how to handle arrays properly
    //TODO: Figure out a way to configure the ledc channels within a function => how to handle arrays properly
    //TODO: Figure out a way to configure the ledc channels within a function => how to handle arrays properly

    // And integrate PWM code from Hello-Esp32
    // Dont forget to remove pin settings from configure_pins()
    ledc_channel_config_t channel_red = ledc_channel[0];
    ledc_channel_config_t channel_green = ledc_channel[1];
    ledc_channel_config_t channel_blue = ledc_channel[2];

    ledc_channel_config(&channel_red);
    ledc_channel_config(&channel_green);
    ledc_channel_config(&channel_blue);

    ledc_fade_func_install(0);
}


void app_main(void)
{
    configure_logging();
    configure_ledc();

    create_wifi_station();
    setup_mqtt_connection();
}