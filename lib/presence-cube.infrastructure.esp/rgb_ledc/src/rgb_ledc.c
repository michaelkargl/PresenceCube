#include "rgb_ledc.h"
#include "logger.h"
#include "math_util.h"
#include "esp_err.h"
#include "ledc_facade.h"
#include "ledc_facade_funcs.h"
#include "exception_handling.h"

#define RGB_8BIT_COLOR_TO_PERCENT(color) ((100 / 255.0f) * color)

ledc_set_percent_func_t rgb_ledc__ledc_facade__set_percent_func = ledc_facade__set_percent;

void set_leds_color_percent(
    const struct ledc_rgb_led_t *leds,
    const int leds_size,
    int percent_red,
    int percent_green,
    int percent_blue,
    uint32_t fade_milliseconds)
{
    THROW_ARGUMENT_NULL_IF_NULL(leds);
    for (int i = 0; i < leds_size; i++)
    {
        const struct ledc_rgb_led_t *led = leds + i;
        set_led_color_percent(led, percent_red, percent_green, percent_blue, fade_milliseconds);
    }
}

void set_led_color_percent(
    const struct ledc_rgb_led_t *led,
    int percent_red,
    int percent_green,
    int percent_blue,
    uint32_t fade_milliseconds)
{
    THROW_ARGUMENT_NULL_IF_NULL(led);
    THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT_REF(led);

    percent_red = ranged_value(percent_red, 0, 100);
    percent_green = ranged_value(percent_green, 0, 100);
    percent_blue = ranged_value(percent_blue, 0, 100);
    LOG_DEBUG("Setting led %s to color: %i, %i, %i percent\n", led->name, percent_red, percent_green, percent_blue);

    rgb_ledc__ledc_facade__set_percent_func(&led->red, percent_red, fade_milliseconds);
    rgb_ledc__ledc_facade__set_percent_func(&led->green, percent_green, fade_milliseconds);
    rgb_ledc__ledc_facade__set_percent_func(&led->blue, percent_blue, fade_milliseconds);
}

void set_led_color_8bit(
    const struct ledc_rgb_led_t *led,
    uint8_t red,
    uint8_t green,
    uint8_t blue,
    uint32_t fade_milliseconds)
{

    THROW_ARGUMENT_NULL_IF_NULL(led);
    THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT_REF(led);

    set_led_color_percent(
        led,
        RGB_8BIT_COLOR_TO_PERCENT(red),
        RGB_8BIT_COLOR_TO_PERCENT(green),
        RGB_8BIT_COLOR_TO_PERCENT(blue),
        fade_milliseconds);
}
