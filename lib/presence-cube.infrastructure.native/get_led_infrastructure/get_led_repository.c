#include "stddef.h"
#include "get_led_repository.h"

// TODO: move to infrastructure

const rgb_led_domain_bag_t _registered_leds = {
    .is_initialized = true,
    .count = 2,
    .leds = (rgb_led_domain_t[2]){
        {
            .id = 0,
            .is_initialized = true,
            .display_name = "left"
        },
        {
            .id = 0,
            .is_initialized = true,
            .display_name = "right"
        }
    }
};

const rgb_led_domain_bag_t* get_leds() {
    return &_registered_leds;
}