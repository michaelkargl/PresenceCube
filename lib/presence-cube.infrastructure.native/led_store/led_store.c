#include "led_store.h"
#include "exception_handling.h"

#define LED_STORE__LED_COUNT 2
#define ENSURE_MODULE_INITIALIZED() THROW_IF_FALSY( \
    ERROR_CODE_UNINITIALIZED_MODULE_ACCESS,         \
    led_store__initialized(),                       \
    "An attempt was made to access an uninitialized module: led_store.");

/**
 * @brief A list of virtual LEDs.
 *        If we imagine the native application to be a virtual device,
 *        these were the LEDs accessable via GPIO pins.
 */
static rgb_led_domain_bag_t registered_leds = {
    .count = LED_STORE__LED_COUNT,
    .leds = (rgb_led_diode_t[LED_STORE__LED_COUNT]){}};

static const led_diode_t build_led(uint8_t id, const char *name)
{
    led_diode_t led = {
        .is_initialized = true,
        .id = id,
        .brightness_percent = 0};

    strncpy(led.display_name, name, sizeof(led.display_name));
    return led;
}

static const rgb_led_diode_t build_rgb_led(uint8_t id, const char *name)
{
    rgb_led_diode_t led = {
        .is_initialized = true,
        .id = id,
        .red = build_led(0, "red"),
        .green = build_led(1, "green"),
        .blue = build_led(2, "blue")};

    strncpy(led.display_name, name, sizeof(led.display_name));
    return led;
}

bool led_store__initialized()
{
    return registered_leds.is_initialized;
}

void led_store__initialize()
{
    LOG_INFORMATION("Initializing module...");
    rgb_led_diode_t leds[LED_STORE__LED_COUNT] = {
        build_rgb_led(0, "east"),
        build_rgb_led(1, "west")};

    memcpy(registered_leds.leds, leds, sizeof(leds));
    registered_leds.is_initialized = true;
}

const rgb_led_domain_bag_t *led_store__get_leds()
{
    ENSURE_MODULE_INITIALIZED();
    return &registered_leds;
}

const rgb_led_diode_t *led_store__get_led(uint8_t id)
{
    ENSURE_MODULE_INITIALIZED();

    const rgb_led_domain_bag_t *led_bag = led_store__get_leds();
    for (uint8_t i = 0; i < led_bag->count; i++)
    {
        rgb_led_diode_t *led = led_bag->leds + i;
        if (led->id == id)
        {
            return led;
        }
    }

    return NULL;
}

void led_store__update(rgb_led_diode_t led)
{
    ENSURE_MODULE_INITIALIZED();

    rgb_led_diode_t *mutatable_led = (rgb_led_diode_t *)led_store__get_led(led.id);
    THROW_RESOURCE_NOT_FOUND_IF_NULL(mutatable_led, "Physical led %u could not be found.", led.id);

    LOG_INFORMATION(
        "Setting LED %u from color (%u, %u, %u)%% => (%u, %u, %u)%%.",
        mutatable_led->id,
        mutatable_led->red.brightness_percent,
        mutatable_led->green.brightness_percent,
        mutatable_led->blue.brightness_percent,
        led.red.brightness_percent,
        led.green.brightness_percent,
        led.blue.brightness_percent);

    memcpy(mutatable_led, &led, sizeof(*mutatable_led));
}