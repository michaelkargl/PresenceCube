#include "rgb_ledc_adapter.h"
#include "logger.h"
#include "exception_handling.h"
#include "led_store.h"

static bool module_initialized = false;

void rgb_ledc_adapter__initialize()
{
    if (!module_initialized)
    {
        led_store__initialize();
        module_initialized = true;
    }
}

bool rgb_ledc_adapter__initialized()
{
    return module_initialized;
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
}