#include "led_store.h"
#include "ledc_rgb_led_builder.h"
#include "string.h"
#include "exception_handling.h"

#define ENSURE_MODULE_INITIALIZED() THROW_IF_FALSY( \
    ERROR_CODE_UNINITIALIZED_MODULE_ACCESS,         \
    led_store__initialized(),                       \
    "An attempt was made to access an uninitialized module: led_store.");

// TODO: validate this working by testing it out at runtime (vscode is unreliable)
#ifdef HARDWARE_RGB_LED_TYPE_COMMON_ANODE
#define RGBA_LED_TYPE LED_TYPE__COMMON_ANODE
#else
#define RGBA_LED_TYPE LED_TYPE__COMMON_CATHODE
#endif

#define RGBA_EAST_RED_GPIO_PIN (CONFIG_RGBA_EAST_RED_CHANNEL_PIN)
#define RGBA_EAST_GREEN_GPIO_PIN (CONFIG_RGBA_EAST_GREEN_CHANNEL_PIN)
#define RGBA_EAST_BLUE_GPIO_PIN (CONFIG_RGBA_EAST_BLUE_CHANNEL_PIN)

#define RGBA_WEST_RED_GPIO_PIN (CONFIG_RGBA_WEST_RED_CHANNEL_PIN)
#define RGBA_WEST_GREEN_GPIO_PIN (CONFIG_RGBA_WEST_GREEN_CHANNEL_PIN)
#define RGBA_WEST_BLUE_GPIO_PIN (CONFIG_RGBA_WEST_BLUE_CHANNEL_PIN)
#define LED_STORE__RGB_LED_COUNT CONFIG_CUBE_HARDWARE_RGB_LED_COUNT

static struct ledc_rgb_led_t leds[LED_STORE__RGB_LED_COUNT];

struct ledc_rgb_led_t (*led_store__build_ledc_rgb_led)(
    const char name[10],
    const ledc_timer_config_t timer,
    const struct ledc_rgb_channels_t channels,
    const struct ledc_rgb_gpio_pins_t gpio_pins,
    led_type_t led_type) = build_ledc_rgb_led;

static void store_led(uint8_t index, struct ledc_rgb_led_t led)
{
    const uint8_t array_length = led_store__get_led_count();
    THROW_IF_TRUTHY(
        ERROR_CODE_INDEX_OUT_OF_RANGE,
        index >= array_length,
        "The requested index %u falls outside of the range of 0-%u!",
        index, array_length - 1);

    leds[index] = led;
}

static ledc_timer_config_t build_low_speed_ledc_timer()
{
    return (ledc_timer_config_t){
        .duty_resolution = LEDC_TIMER_13_BIT, // 13bit = 2^13 = 81 92 levels within 1 cycle
        .freq_hz = 5000,                      // 5kHz = 1 cycle lasts 1/5000s
        .speed_mode = LEDC_LOW_SPEED_MODE,    // Note: ESP32-S2 only supports “low speed” channels
        .timer_num = LEDC_TIMER_0,            // PWM timer
        .clk_cfg = LEDC_AUTO_CLK              // Auto select the source clock
    };
}

static struct ledc_rgb_led_t build_ledc_rgb_led_east()
{
    return build_ledc_rgb_led(
        "east",
        build_low_speed_ledc_timer(),
        (struct ledc_rgb_channels_t){
            .red = LEDC_CHANNEL_0,
            .green = LEDC_CHANNEL_1,
            .blue = LEDC_CHANNEL_2},
        (struct ledc_rgb_gpio_pins_t){
            .red = RGBA_EAST_RED_GPIO_PIN,
            .green = RGBA_EAST_GREEN_GPIO_PIN,
            .blue = RGBA_EAST_BLUE_GPIO_PIN},
        RGBA_LED_TYPE);
}

static struct ledc_rgb_led_t build_ledc_rgb_led_west()
{
    return build_ledc_rgb_led(
        "west",
        build_low_speed_ledc_timer(),
        (struct ledc_rgb_channels_t){
            .red = LEDC_CHANNEL_3,
            .green = LEDC_CHANNEL_4,
            .blue = LEDC_CHANNEL_5},
        (struct ledc_rgb_gpio_pins_t){
            .red = RGBA_WEST_RED_GPIO_PIN,
            .green = RGBA_WEST_GREEN_GPIO_PIN,
            .blue = RGBA_WEST_BLUE_GPIO_PIN},
        RGBA_LED_TYPE);
}

static bool all_leds_initialized()
{
    struct ledc_rgb_led_t *leds = led_store__get_leds();
    THROW_ARGUMENT_NULL_IF_NULL(leds);

    for (uint8_t i = 0; i < led_store__get_led_count(); i++)
    {
        struct ledc_rgb_led_t *led = leds + i;
        THROW_ARGUMENT_NULL_IF_NULL(led);

        if (!led->is_initialized)
        {
            return false;
        }
    }
    return true;
}

// public functions

esp_err_t led_store__initialize()
{
    store_led(0, build_ledc_rgb_led_east());
    store_led(1, build_ledc_rgb_led_west());
    return ESP_OK;
}

bool led_store__initialized()
{
    return all_leds_initialized();
}

uint8_t led_store__get_led_count()
{
    return sizeof(leds) / sizeof(leds[0]);
}

struct ledc_rgb_led_t *led_store__get_leds()
{
    return leds;
}

static bool try_get_index_of_led(uint8_t id, uint8_t *out_index)
{
    struct ledc_rgb_led_t *leds = led_store__get_leds();
    THROW_ARGUMENT_NULL_IF_NULL(leds);

    for (uint8_t i = 0; i < led_store__get_led_count(); i++)
    {
        struct ledc_rgb_led_t *led = leds + i;
        THROW_ARGUMENT_NULL_IF_NULL(led);

        if (led->id == id)
        {
            *out_index = i;
            return true;
        }
    }

    return false;
}

struct ledc_rgb_led_t *led_store__get_led(uint8_t id)
{
    ENSURE_MODULE_INITIALIZED();

    uint8_t index = 0;
    if (!try_get_index_of_led(id, &index))
    {
        return NULL;
    }

    return leds + index;
}

void led_store__update(struct ledc_rgb_led_t led)
{
    ENSURE_MODULE_INITIALIZED();

    uint8_t index;
    THROW_IF_FALSY(
        ERROR_CODE_RESOURCE_NOT_FOUND,
        try_get_index_of_led(led.id, &index),
        "Unable to store LED. No instance with id=%u could not be found.",
        led.id);

    store_led(index, led);
}