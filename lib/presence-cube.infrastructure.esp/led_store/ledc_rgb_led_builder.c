#include "ledc_rgb_led_builder.h"
#include "ledc_led_builder.h"
#include "string.h"

/**
 * @brief ledc_led builder facade
 */
struct ledc_led_t (*ledc_rgb_led_builder__build_ledc_led)(
    char name[10],
    ledc_timer_config_t timer,
    ledc_channel_t channel,
    uint8_t gpio_pin,
    led_type_t led_type
) = build_ledc_led;


static uint8_t id_counter = 0;

static unsigned int _generate_id() {
    return id_counter++;
}

struct ledc_rgb_led_t build_ledc_rgb_led(
    const char name[10],
    const ledc_timer_config_t timer,
    const struct ledc_rgb_channels_t channels,
    const struct ledc_rgb_gpio_pins_t gpio_pins,
    led_type_t led_type
) {
    struct ledc_rgb_led_t led = {
        .id = _generate_id(),
        .red = ledc_rgb_led_builder__build_ledc_led("red", timer, channels.red, gpio_pins.red, led_type),
        .green = ledc_rgb_led_builder__build_ledc_led("green", timer, channels.green, gpio_pins.green, led_type),
        .blue = ledc_rgb_led_builder__build_ledc_led("blue", timer, channels.blue, gpio_pins.blue, led_type)
    };  

    strncpy(led.name, name, sizeof(led.name));
    led.is_initialized = true;

    return led;
}