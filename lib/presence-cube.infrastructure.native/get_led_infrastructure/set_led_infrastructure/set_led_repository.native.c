#include "stddef.h"
#include "set_led_repository.h"
#include "logger.h"
#include "led_store.h"
#include "exception_handling.h"

led_store__update_func_t set_led_repository__led_store__update_led = led_store__update;
led_store__get_led_func_t set_led_repository__led_store__get_led = led_store__get_led;


// TODO: move this into the module containing rgb_led_diode_t
static rgb_led_diode_t copy_led(const rgb_led_diode_t* source_led) {
    rgb_led_diode_t target_copy;
    
    const uint8_t source_type_size = sizeof(*source_led);
    const uint8_t target_type_size = sizeof(target_copy);
    const bool types_match = target_type_size == source_type_size;
    
    THROW_IF_FALSY(
        ERROR_CODE_MEMCOPYING_MISMATCHING_TYPES,
        types_match,
        "Attempting to copy a type of size %ubyte onto a type with size %ubyte.",
        source_type_size,
        target_type_size
    );

    memcpy(&target_copy, source_led, target_type_size);
    return target_copy;
}

void set_led_repository__update(
    uint8_t id,
    uint8_t red_percent,
    uint8_t green_percent,
    uint8_t blue_percent)
{
    const rgb_led_diode_t* led = set_led_repository__led_store__get_led(id);
    THROW_RESOURCE_NOT_FOUND_IF_NULL(led, "An LED with id=%u could not be found.", id);
    
    // To keep this function idempotent, we should not mutate global
    // state just to apply an update to a data store.
    rgb_led_diode_t led_copy = copy_led(led);
    led_copy.red.brightness_percent = red_percent;
    led_copy.green.brightness_percent = green_percent;
    led_copy.blue.brightness_percent = blue_percent;

    set_led_repository__led_store__update_led(led_copy);
}