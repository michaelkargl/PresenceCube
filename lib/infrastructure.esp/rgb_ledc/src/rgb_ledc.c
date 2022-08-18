#include "rgb_ledc.h"
#include "logger.h"
#include "math_util.h"
#include "rgb_ledc_duty_calculator.h"
// TODO: replace esp specific error handling with custom logic
#include "esp_err.h"
#include "driver/ledc.h"

// Prototypes
// TODO: replace concrete esp specific types with custom dtos to separate domain from infrastructure
static void _set_color_soft(const ledc_channel_config_t *channel, int32_t duty, int32_t fade_time_ms);
static void _set_color_hard(const ledc_channel_config_t *channel, int32_t duty);
static void _set_led_color(const struct ledc_led_t *led, int32_t duty, int32_t fade_time_ms);
static void _set_rgb_led_color(
    const struct ledc_rgb_led_t *led,
    int32_t duty_red,
    int32_t duty_green,
    int32_t duty_blue,
    uint32_t fade_milliseconds);

// Globals
static const char *TAG = "rgb_ledc";

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
    int32_t duty_blue,
    uint32_t fade_milliseconds
)
{
    _set_led_color(&led->red, duty_red, fade_milliseconds);
    _set_led_color(&led->green, duty_green, fade_milliseconds);
    _set_led_color(&led->blue, duty_blue, fade_milliseconds);
}

void set_leds_color_percent(
    const struct ledc_rgb_led_t *leds,
    const int leds_size,
    int percent_red,
    int percent_green,
    int percent_blue,
    uint32_t fade_milliseconds
) {
    for ( int i = 0; i < leds_size; i++ ) {
        const struct ledc_rgb_led_t led = leds[i];
        if (led.is_initialized) {
            log_information(TAG, "Toggling led %s to color (%i,%i,%i)\n", led.name, percent_red, percent_blue, percent_green);
            set_led_color_percent(&led, percent_red, percent_green, percent_blue, fade_milliseconds);
        } else {
            log_warning(TAG, "Ignoring request to toggle led %s. Led is uninitialized!\n", led.name);
        }
    }
}

void set_led_color_percent(
    const struct ledc_rgb_led_t *led,
    int percent_red,
    int percent_green,
    int percent_blue,
    uint32_t fade_milliseconds)
{
    percent_red = ranged_value(percent_red, 0, 100);
    percent_green = ranged_value(percent_green, 0, 100);
    percent_blue = ranged_value(percent_blue, 0, 100);
    log_debug(TAG, "Setting led %s to color: %i, %i, %i percent\n", led->name, percent_red, percent_green, percent_blue);
    
    int duty_red = calculate_duty(&led->red, percent_red);
    int duty_green = calculate_duty(&led->green, percent_green);
    int duty_blue = calculate_duty(&led->blue, percent_blue);

    _set_rgb_led_color(
        led,
        duty_red,
        duty_green,
        duty_blue,
        fade_milliseconds);
}

void set_led_color_8bit(
    const struct ledc_rgb_led_t *led,
    uint8_t red,
    uint8_t green,
    uint8_t blue,
    uint32_t fade_milliseconds)
{
    double calculate_percent(uint8_t color) {
        return (100 / (double) 255) * color;
    };

    set_led_color_percent(
        led,
        calculate_percent(red),
        calculate_percent(green),
        calculate_percent(blue),
        fade_milliseconds
    );
}

static void _set_color_hard(const ledc_channel_config_t *channel, int32_t duty)
{
    if (duty < 0)
        return;

    // configure channel updates
    log_debug(TAG, "Switching duty cycle of channel %d from %d => %d\n", channel->channel, channel->duty, duty);
    ESP_ERROR_CHECK(ledc_set_duty(
        channel->speed_mode,
        channel->channel,
        duty));

    // activate changes
    // TODO: move the setting of ledc into an adapter that can translate between dto<->domain and esp_err<->custom_err codes
    ESP_ERROR_CHECK(ledc_update_duty(channel->speed_mode, channel->channel));
}

static void _set_color_soft(const ledc_channel_config_t *channel, int32_t duty, int32_t fade_time_ms)
{
    if (duty < 0)
        return;

    log_debug(TAG, "Softly setting duty cycle of channel %d from %d => %d over %d ms\n",
             channel->channel, channel->duty, duty, fade_time_ms);

    // TODO: move the setting of ledc into an adapter that can translate between dto<->domain and esp_err<->custom_err codes
    ESP_ERROR_CHECK(ledc_set_fade_with_time(channel->speed_mode, channel->channel, duty, fade_time_ms));
    ESP_ERROR_CHECK(ledc_fade_start(channel->speed_mode, channel->channel, LEDC_FADE_NO_WAIT));
}
