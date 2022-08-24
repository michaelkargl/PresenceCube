#include "led_store.h"
#include "ledc_rgb_led_builder.h"
#include "string.h"
#include "CException.h"


#define RGBA_EAST_RED_GPIO_PIN (CONFIG_RGBA_EAST_RED_CHANNEL_PIN)
#define RGBA_EAST_GREEN_GPIO_PIN (CONFIG_RGBA_EAST_GREEN_CHANNEL_PIN)
#define RGBA_EAST_BLUE_GPIO_PIN (CONFIG_RGBA_EAST_BLUE_CHANNEL_PIN)

#define RGBA_WEST_RED_GPIO_PIN (CONFIG_RGBA_EAST_RED_CHANNEL_PIN)
#define RGBA_WEST_GREEN_GPIO_PIN (CONFIG_RGBA_EAST_GREEN_CHANNEL_PIN)
#define RGBA_WEST_BLUE_GPIO_PIN (CONFIG_RGBA_EAST_BLUE_CHANNEL_PIN)


#define LED_STORE__RGB_LED_COUNT CONFIG_CUBE_HARDWARE_RGB_LED_COUNT



static struct ledc_rgb_led_t _leds[LED_STORE__RGB_LED_COUNT];

struct ledc_rgb_led_t (*_led_store__build_ledc_rgb_led)(
    const char name[10],
    const ledc_timer_config_t timer,
    const struct ledc_rgb_channels_t channels,
    const struct ledc_rgb_gpio_pins_t gpio_pins,
    bool is_common_anode) = build_ledc_rgb_led;
    


static void _store_led(uint8_t index, struct ledc_rgb_led_t led)
{
    if (index >= led_store__get_led_count()) {
        Throw(ERROR_CODE_INDEX_OUT_OF_RANGE);
    }
    
    _leds[index] = led;
}

static ledc_timer_config_t _build_low_speed_ledc_timer()
{
    return (ledc_timer_config_t){
        .duty_resolution = LEDC_TIMER_13_BIT, // 13bit = 2^13 = 81 92 levels within 1 cycle
        .freq_hz = 5000,                      // 5kHz = 1 cycle lasts 1/5000s
        .speed_mode = LEDC_LOW_SPEED_MODE,    // Note: ESP32-S2 only supports “low speed” channels
        .timer_num = LEDC_TIMER_0,            // PWM timer
        .clk_cfg = LEDC_AUTO_CLK              // Auto select the source clock
    };
}

static struct ledc_rgb_led_t _build_ledc_rgb_led_east()
{
    return build_ledc_rgb_led(
        "east",
        _build_low_speed_ledc_timer(),
        (struct ledc_rgb_channels_t){
            .red = LEDC_CHANNEL_0,
            .green = LEDC_CHANNEL_1,
            .blue = LEDC_CHANNEL_2},
        (struct ledc_rgb_gpio_pins_t){
            .red = RGBA_EAST_RED_GPIO_PIN,
            .green = RGBA_EAST_GREEN_GPIO_PIN,
            .blue = RGBA_EAST_BLUE_GPIO_PIN},
        true /* TODO: to enum */
    );
}

static struct ledc_rgb_led_t _build_ledc_rgb_led_west()
{
    return build_ledc_rgb_led(
        "west",
        _build_low_speed_ledc_timer(),
        (struct ledc_rgb_channels_t){
            .red = LEDC_CHANNEL_3,
            .green = LEDC_CHANNEL_4,
            .blue = LEDC_CHANNEL_5},
        (struct ledc_rgb_gpio_pins_t){
            .red = RGBA_WEST_RED_GPIO_PIN,
            .green = RGBA_WEST_GREEN_GPIO_PIN,
            .blue = RGBA_WEST_BLUE_GPIO_PIN},
        true /* TODO: to enum */
    );
}

// public functions

esp_err_t led_store_initialize()
{
    // TODO: this can be made a compile time constant (there are no moving parts)
    _store_led(0, _build_ledc_rgb_led_east());
    _store_led(1, _build_ledc_rgb_led_west());
    return ESP_OK;
}

uint8_t led_store__get_led_count()
{
    return sizeof(_leds) / sizeof(_leds[0]);
}

struct ledc_rgb_led_t *led_store__get_leds()
{
    return _leds;
}