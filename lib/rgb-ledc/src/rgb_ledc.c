#include "rgb_ledc.h"
#include "esp_log.h"
#include "math_util.h"
#include "rgb_ledc_duty_calculator.h"

// Prototypes
static void _set_color_soft(const ledc_channel_config_t *channel, int32_t duty, int32_t fade_time_ms);
static void _set_color_hard(const ledc_channel_config_t *channel, int32_t duty);
static void _set_led_color(const struct ledc_led_t *led, int32_t duty, int32_t fade_time_ms);
static void _set_rgb_led_color(
    const struct ledc_rgb_led_t *led,
    int32_t duty_red,
    int32_t duty_green,
    int32_t duty_blue);

// Globals
static const char *TAG = "ledc";

static void _set_led_color(
    const struct ledc_led_t *led,
    int32_t duty,
    int32_t fade_time_ms)
{

    if (fade_time_ms > 0)
    {
        _set_color_soft(&led->channel, duty, fade_time_ms);
    }
    else
    {
        _set_color_hard(&led->channel, duty);
    }
}

static void _set_rgb_led_color(
    const struct ledc_rgb_led_t *led,
    int32_t duty_red,
    int32_t duty_green,
    int32_t duty_blue)
{
    _set_led_color(&led->red, duty_red, led->fade_milliseconds);
    _set_led_color(&led->green, duty_green, led->fade_milliseconds);
    _set_led_color(&led->blue, duty_blue, led->fade_milliseconds);
}

void set_leds_color_percent(
    const struct ledc_rgb_led_t *leds,
    const int leds_size,
    int percent_red,
    int percent_green,
    int percent_blue
) {
    for ( int i = 0; i < leds_size; i++ ) {
        const struct ledc_rgb_led_t led = leds[i];
        if (led.is_initialized) {
            ESP_LOGI(TAG, "Toggling led %s to color (%i,%i,%i)", led.name, percent_red, percent_blue, percent_green);
            set_led_color_percent(&led, percent_red, percent_green, percent_blue);
        } else {
            ESP_LOGW(TAG, "Ignoring request to toggle led %s. Led is uninitialized!", led.name);
        }
    }
}

void set_led_color_percent(
    const struct ledc_rgb_led_t *led,
    int percent_red,
    int percent_green,
    int percent_blue)
{
    percent_red = ranged_value(percent_red, 0, 100);
    percent_green = ranged_value(percent_green, 0, 100);
    percent_blue = ranged_value(percent_blue, 0, 100);
    ESP_LOGD(TAG, "Setting led %s to color: %i, %i, %i percent", led->name, percent_red, percent_green, percent_blue);
    
    if ( led->is_common_anode) {
        // less gpio backpressure = more flow through the led
        percent_red = 100 - percent_red;
        percent_green = 100 - percent_green;
        percent_blue = 100 - percent_blue;
    }

    int duty_red = _calculate_duty(&led->red, percent_red);
    int duty_green = _calculate_duty(&led->green, percent_green);
    int duty_blue = _calculate_duty(&led->blue, percent_blue);

    _set_rgb_led_color(
        led,
        duty_red,
        duty_green,
        duty_blue);
}

void set_led_color_8bit(
    const struct ledc_rgb_led_t *led,
    uint8_t red,
    uint8_t green,
    uint8_t blue)
{
    double calculate_percent(uint8_t color) {
        return (100 / (double) 255) * color;
    };

    set_led_color_percent(
        led,
        calculate_percent(red),
        calculate_percent(green),
        calculate_percent(blue)
    );
}

static void _set_color_hard(const ledc_channel_config_t *channel, int32_t duty)
{
    if (duty < 0)
        return;

    // configure channel updates
    ESP_LOGD(TAG, "Switching duty cycle of channel %d from %d => %d", channel->channel, channel->duty, duty);
    ESP_ERROR_CHECK(ledc_set_duty(
        channel->speed_mode,
        channel->channel,
        duty));

    // activate changes
    ESP_ERROR_CHECK(ledc_update_duty(channel->speed_mode, channel->channel));
}

static void _set_color_soft(const ledc_channel_config_t *channel, int32_t duty, int32_t fade_time_ms)
{
    if (duty < 0)
        return;

    ESP_LOGD(TAG, "Softly setting duty cycle of channel %d from %d => %d over %d ms",
             channel->channel, channel->duty, duty, fade_time_ms);
    ESP_ERROR_CHECK(ledc_set_fade_with_time(channel->speed_mode, channel->channel, duty, fade_time_ms));
    ESP_ERROR_CHECK(ledc_fade_start(channel->speed_mode, channel->channel, LEDC_FADE_NO_WAIT));
}
