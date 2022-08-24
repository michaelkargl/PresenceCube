#include "get_led_repository.h"
#include "led_store.h"
#include "map_ledc_led__led_domain.h"
#include "logger.h"

#define GET_LED_REPOSITORY__LED_BAG_SIZE CONFIG_CUBE_HARDWARE_RGB_LED_COUNT

//
// PLATFORM: ESP
// 

static rgb_led_domain_bag_t _led_bag = {
    .leds = (rgb_led_domain_t[GET_LED_REPOSITORY__LED_BAG_SIZE]) {},
    .count = GET_LED_REPOSITORY__LED_BAG_SIZE,
    .is_initialized = true
};

const rgb_led_domain_bag_t* get_leds() {
    // TODO: this could be done in an initializer or cached
    map_rgb_ledc_led_array__to__rgb_led_domain_bag(
        led_store__get_leds(),
        led_store__get_led_count(),
        &_led_bag
    );
    return &_led_bag;
}