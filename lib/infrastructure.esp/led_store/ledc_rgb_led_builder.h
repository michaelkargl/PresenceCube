#include "ledc_rgb_gpio_pins_t.h"
#include "ledc_rgb_channels_t.h"
#include "ledc_rgb_led_t.h"
#include "hal/ledc_types.h"

struct ledc_rgb_led_t build_ledc_rgb_led(
    const char name[10],
    const ledc_timer_config_t timer,
    const struct ledc_rgb_channels_t channels,
    const struct ledc_rgb_gpio_pins_t gpio_pins,
    bool is_common_anode
);