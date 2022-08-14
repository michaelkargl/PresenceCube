#include "ledc_channel_config_builder.h"

ledc_channel_config_t build_ledc_channel_config(
    ledc_channel_t channel,
    ledc_timer_config_t timer,
    int gpio_pin
) {
    return (ledc_channel_config_t) {
        .channel = channel,
        .gpio_num = gpio_pin,
        .speed_mode = timer.speed_mode,
        .hpoint = 0,
        .timer_sel = timer.timer_num,
        .intr_type = LEDC_INTR_DISABLE,
    };
}