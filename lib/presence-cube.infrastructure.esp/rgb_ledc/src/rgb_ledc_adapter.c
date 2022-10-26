#include "rgb_ledc_adapter.h"
#include "logger.h"
#include "rgb_ledc_init.h"
#include "rgb_ledc.h"
#include "exception_handling.h"
#include "led_store.h"

// TODO: configure using menuconfig
#define LED_FADE_MILLISECONDS 1000

led_store__get_led_func_t rgb_ledc_adapter__led_store__get_led_fn = led_store__get_led;
rgb_ledc__set_led_color_percent_func_t rgb_ledc_adapter__rgb_ledc__set_led_color_percent_fn = set_led_color_percent;

static bool _module_initialized = false;

void rgb_ledc_adapter__initialize()
{
    if (!rgb_ledc_adapter__initialized())
    {

        led_store__initialize();
        configure_rgb_leds(
            led_store__get_leds(),
            led_store__get_led_count());
        _module_initialized = true;
    }
}

bool rgb_ledc_adapter__initialized()
{
    return _module_initialized;
}

void rgb_ledc_adapter__set_color(
    const rgb_led_domain_t *led,
    uint8_t percent_red,
    uint8_t percent_green,
    uint8_t percent_blue)
{
    THROW_ARGUMENT_NULL_IF_NULL(led);
    THROW_IF_FALSY(
        ERROR_CODE_UNINITIALIZED_MODULE_ACCESS,
        rgb_ledc_adapter__initialized(),
        "Illegal access to uninitialized module: rgb_ledc_adapter");

    LOG_INFORMATION(
        "Setting LED %u from color (%u, %u, %u)%% => (%u, %u, %u)%%.",
        led->id,
        led->red.brightness_percent,
        led->green.brightness_percent,
        led->blue.brightness_percent,
        percent_red,
        percent_green,
        percent_blue);

    struct ledc_rgb_led_t *ledc_led = rgb_ledc_adapter__led_store__get_led_fn(led->id);
    THROW_RESOURCE_NOT_FOUND_IF_NULL(ledc_led, "Led with id %i could not be found.", led->id);

    rgb_ledc_adapter__rgb_ledc__set_led_color_percent_fn(
        ledc_led,
        percent_red,
        percent_green,
        percent_blue,
        LED_FADE_MILLISECONDS);
}