#include "ledc_rgb_led_builder.h"


struct ledc_rgb_led_t stub__build_ledc_rgb_led(
    const char name[10],
    const ledc_timer_config_t timer,
    const struct ledc_rgb_channels_t channels,
    const struct ledc_rgb_gpio_pins_t gpio_pins,
    bool is_common_anode
) {
    struct ledc_rgb_led_t dummy = {};
    return dummy;
}