#include "led_register.h"

// TODO: move to infrastructure

const led_domain_t _registered_leds[2] = {
    {
        .initialized = true,
        .id = 0,
        .brightness_percent = 0,
        .display_name = "left",
    },
    {
        .initialized = true,
        .id = 1,
        .brightness_percent = 0,
        .display_name = "right",
    }
};

const led_domain_bag_t get_leds() {
    return (led_domain_bag_t) {
        .leds = _registered_leds,
        .count = sizeof(_registered_leds) / sizeof(led_domain_t)
    };
}