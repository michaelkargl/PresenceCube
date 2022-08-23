// TODO: rename to repository
#include "get_led_repository.h"
#include "led_store.h"


const led_domain_t _registered_leds[2] = {
    {
        .is_initialized = true,
        .id = 4,
        .brightness_percent = 0,
        .display_name = "left",
    },
    {
        .is_initialized = true,
        .id = 5,
        .brightness_percent = 0,
        .display_name = "right",
    }
};

const led_domain_bag_t get_leds() {
    led_store_initialize();
    
    return (led_domain_bag_t) {
        .leds = _registered_leds,
        .count = sizeof(_registered_leds) / sizeof(led_domain_t)
    };
}