#include "get_led_repository.h"
#include "led_store.h"
#include "logger.h"

led_store__get_led_func_t get_led_repository__get_led_fn = led_store__get_led;
led_store__get_leds_func_t get_led_repository__get_leds_fn = led_store__get_leds;

const rgb_led_domain_bag_t *get_led_repository__get_leds()
{
    LOG_DEBUG("Fetching all available LEDs...");
    return get_led_repository__get_leds_fn();
}

const rgb_led_diode_t *get_led_repository__get_led(uint8_t id)
{
    LOG_DEBUG("Finding LED with id=%u.", id);
    return get_led_repository__get_led_fn(id);
}