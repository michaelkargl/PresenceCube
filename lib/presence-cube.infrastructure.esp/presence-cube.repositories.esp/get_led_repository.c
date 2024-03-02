#include "get_led_repository.h"
#include "led_store.h"
#include "map_ledc_led__led_domain.h"
#include "throwing_helpers.h"
#include "logger.h"

#define GET_LED_REPOSITORY__LED_BAG_SIZE CONFIG_CUBE_HARDWARE_RGB_LED_COUNT

// TODO #121: Create a test for the presence-cube.repositories.esp/get_led_repository
//
// PLATFORM: ESP
//

static rgb_led_domain_bag_t led_bag = {
    .leds = (rgb_led_diode_t[GET_LED_REPOSITORY__LED_BAG_SIZE]){},
    .count = GET_LED_REPOSITORY__LED_BAG_SIZE,
    .is_initialized = true};

const rgb_led_domain_bag_t *get_led_repository__get_leds()
{
    map_rgb_ledc_led_array__to__rgb_led_domain_bag(
        led_store__get_leds(),
        led_store__get_led_count(),
        &led_bag);
    return &led_bag;
}

const rgb_led_diode_t *get_led_repository__get_led(uint8_t id)
{
    const rgb_led_domain_bag_t *led_bag = get_led_repository__get_leds();
    THROW_IF_TRUTHY(
        ERROR_CODE_ARGUMENT_NULL,
        led_bag == NULL,
        "Unable to find LED with id %u since the repository returned a NULL "
        " list of LEDs. Did you miss an initializer?",
        id);

    for (uint8_t i = 0; i < led_bag->count; i++)
    {
        rgb_led_diode_t *led = led_bag->leds + i;
        if (led->id == id)
        {
            return led;
        }
    }

    return NULL;
}
