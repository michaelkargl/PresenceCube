#include "zube_ledc.h"
#include "esp_log.h"
#include "math_helper.h"
#include <math.h>

// Prototypes
static int _calculate_duty(int8_t percent);
static void _set_color_soft(ledc_channel_config_t* channel, int32_t duty, int32_t fade_time_ms);
static void _set_color_hard(ledc_channel_config_t* channel, int32_t duty);
static void _set_led_color_channel(ledc_channel_config_t* channel, int32_t duty, int32_t fade_time_ms);
static int _get_max_duty(ledc_timer_config_t* timer);
static void _set_led_color_rgb(
    ledc_channel_config_t *rgb_channels,
    int32_t duty_red,
    int32_t duty_green,
    int32_t duty_blue,
    int32_t fade_time_ms);

// Globals
static const char *TAG = "zube_ledc";
static ledc_timer_config_t _ledc_timer;

//TODO: Extract MQTT into separate file => Provide separate events for each MQTT event
//TODO: Extract LEDC into separate file


static int _get_max_duty(ledc_timer_config_t* timer)
{
    int duty_resolution = (int)timer->duty_resolution;
    int duty = pow(2, duty_resolution) - 1;

    ESP_LOGD(TAG, "Calculated max duty of %d with a resolution of %d", duty, timer->duty_resolution);
    return duty;
}

static void _set_led_color_channel(
    ledc_channel_config_t* channel, int32_t duty, int32_t fade_time_ms) {

    if (fade_time_ms > 0)
    {
        _set_color_soft(channel, duty, fade_time_ms);
    }
    else
    {
        _set_color_hard(channel, duty);
    }
}

static void _set_led_color_rgb(
    ledc_channel_config_t* rgb_channels,
    int32_t duty_red,
    int32_t duty_green,
    int32_t duty_blue,
    int32_t fade_time_ms)
{
    ledc_channel_config_t* channel_red = rgb_channels;
    ledc_channel_config_t* channel_green = rgb_channels + 1;
    ledc_channel_config_t* channel_blue = rgb_channels + 2;

    _set_led_color_channel(channel_red, duty_red, fade_time_ms);
    _set_led_color_channel(channel_green, duty_green, fade_time_ms);
    _set_led_color_channel(channel_blue, duty_blue, fade_time_ms);
}

static int _calculate_duty(int8_t percent)
{
    int max_duty = _get_max_duty(&_ledc_timer);
    int duty = floor((max_duty / (float)100) * percent);
    duty = ranged_value(duty, 0, max_duty);

    ESP_LOGD(TAG, "Calculated duty %d for percent %d", duty, percent);
    return duty;
}

void set_led_color_percent(
    ledc_channel_config_t *channels,
    int percent_red,
    int percent_green,
    int percent_blue,
    int transition_interval_ms)
{
    percent_red = ranged_value(percent_red, 0, 100);
    percent_green = ranged_value(percent_green, 0, 100);
    percent_blue = ranged_value(percent_blue, 0, 100);

    int duty_red = _calculate_duty(percent_red);
    int duty_green = _calculate_duty(percent_green);
    int duty_blue = _calculate_duty(percent_blue);

    _set_led_color_rgb(
        channels,
        duty_red,
        duty_green,
        duty_blue,
        transition_interval_ms);
}

static void _set_color_hard(ledc_channel_config_t* channel, int32_t duty)
{
    if (duty < 0)
        return;

    // configure channel updates
    ESP_LOGD(TAG, "Switching duty cycle of channel %d from %d => %d", channel->channel, channel->duty, duty);
    ledc_set_duty(
        channel->speed_mode,
        channel->channel,
        duty);

    // activate changes
    ledc_update_duty(channel->speed_mode, channel->channel);
}

static void _set_color_soft(ledc_channel_config_t* channel, int32_t duty, int32_t fade_time_ms)
{
    if (duty < 0)
        return;

    ESP_LOGD(TAG, "Softly setting duty cycle of channel %d from %d => %d over %d ms",
             channel->channel, channel->duty, duty, fade_time_ms);
    ledc_set_fade_with_time(channel->speed_mode, channel->channel, duty, fade_time_ms);
    ledc_fade_start(channel->speed_mode, channel->channel, LEDC_FADE_NO_WAIT);
}


void configure_ledc(ledc_channel_config_t *ledc_channels, ledc_timer_config_t ledc_timer)
{
    _ledc_timer = ledc_timer;

    ledc_timer_config(&_ledc_timer);

    ledc_channel_config(&ledc_channels[0]); // red
    ledc_channel_config(&ledc_channels[1]); // green
    ledc_channel_config(&ledc_channels[2]); // blue

    ledc_fade_func_install(0);
}