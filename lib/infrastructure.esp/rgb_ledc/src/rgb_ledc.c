#include "rgb_ledc.h"
#include "logger.h"
#include "math_util.h"
#include "esp_err.h"
#include "ledc_facade.h"

static const char *TAG = "rgb_ledc";


void set_leds_color_percent(
    const struct ledc_rgb_led_t *leds,
    const int leds_size,
    int percent_red,
    int percent_green,
    int percent_blue,
    uint32_t fade_milliseconds
) {
    // TODO: null check
    // TODO: index check

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
    // TODO: null check

    percent_red = ranged_value(percent_red, 0, 100);
    percent_green = ranged_value(percent_green, 0, 100);
    percent_blue = ranged_value(percent_blue, 0, 100);
    log_debug(TAG, "Setting led %s to color: %i, %i, %i percent\n", led->name, percent_red, percent_green, percent_blue);
    
    ledc_set_percent(&led->red, percent_red, fade_milliseconds);
    ledc_set_percent(&led->green, percent_green, fade_milliseconds);
    ledc_set_percent(&led->blue, percent_blue, fade_milliseconds);
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

    // TODO: null check

    set_led_color_percent(
        led,
        calculate_percent(red),
        calculate_percent(green),
        calculate_percent(blue),
        fade_milliseconds
    );
}
