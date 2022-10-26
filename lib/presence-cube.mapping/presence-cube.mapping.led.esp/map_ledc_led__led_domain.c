#include "map_ledc_led__led_domain.h"
#include "exception_handling.h"
#include "string.h"
#include "math.h"
#include "ledc_led_duty_calculator.h"
#include "ledc_led_duty_calculator_func.h"
#include "ledc_led_t.h"

calculate_duty_percent_func_t _map_ledc_led__led_domain__calculate_duty_percent = calculate_brightness_percent;

/**
 * @brief Maps all elements of @ref source_leds to the target buffer @ref target_leds
 *
 * @param source_leds an array of @ref ledc_led_t elements to map
 * @param source_led_count the 1-based length of the @ref source_leds array
 * @param target_leds a buffer target array for mappings of @ref source_leds. This array must be at least of size @ref source_led_count
 * @param target_led_count the 1-based size of the @ref target_leds
 *
 * @exception ERROR_CODE_ARGUMENT_NULL if one of the input arrays is NULL
 * @exception ERROR_CODE_INSUFFICIENT_BUFFER if the @ref target_leds array is not big enough to hold all the mappings of @ref source_leds
 */
static void _map_ledc_led_array__to__led_domain_array(
    const struct ledc_led_t source_leds[], uint8_t source_led_count,
    led_domain_t target_leds[], uint8_t target_led_count)
{
    THROW_ARGUMENT_NULL_IF_NULL(source_leds);
    THROW_ARGUMENT_NULL_IF_NULL(target_leds);

    if (target_led_count < source_led_count)
    {
        LOG_ERROR("Unable to map from source to target: the target buffer is by %i too small.", source_led_count - target_led_count);
        Throw(ERROR_CODE_INSUFFICIENT_BUFFER);
    }

    for (uint8_t i = 0; i < target_led_count; i++)
    {
        target_leds[i] = map_ledc_led__to__led_domain(&source_leds[i]);
    }
}

led_domain_t map_ledc_led__to__led_domain(const struct ledc_led_t *led)
{
    THROW_ARGUMENT_NULL_IF_NULL(led);

    led_domain_t target = {
        .is_initialized = led->is_initialized,
        .brightness_percent = _map_ledc_led__led_domain__calculate_duty_percent(led),
        .id = led->id};

    strncpy(target.display_name, led->name, sizeof(target.display_name));

    return target;
}

void map_ledc_led_array__to__led_domain_bag(
    const struct ledc_led_t source_leds[], uint8_t source_led_count,
    led_domain_bag_t *target_led_bag)
{
    THROW_ARGUMENT_NULL_IF_NULL(source_leds);
    THROW_ARGUMENT_NULL_IF_NULL(target_led_bag);

    _map_ledc_led_array__to__led_domain_array(
        source_leds, source_led_count,
        target_led_bag->leds, target_led_bag->count);
    target_led_bag->count = source_led_count;
}

void map_rgb_ledc_led__to__rgb_led_domain(
    const struct ledc_rgb_led_t* source_led,
    rgb_led_domain_t* target_led
) {
    THROW_ARGUMENT_NULL_IF_NULL(source_led);
    THROW_ARGUMENT_NULL_IF_NULL(target_led);

    target_led->id = source_led->id;
    target_led->red = map_ledc_led__to__led_domain(&source_led->red);
    target_led->green = map_ledc_led__to__led_domain(&source_led->green);
    target_led->blue = map_ledc_led__to__led_domain(&source_led->blue);
    target_led->is_initialized = source_led->is_initialized;
    strncpy(target_led->display_name, source_led->name, sizeof(target_led->display_name));
}


void map_rgb_ledc_led_array__to__rgb_led_domain_bag(
    const struct ledc_rgb_led_t * source_leds, uint8_t source_leds_size,
    rgb_led_domain_bag_t* target_leds
) {
    THROW_ARGUMENT_NULL_IF_NULL(source_leds);
    THROW_ARGUMENT_NULL_IF_NULL(target_leds);
    THROW_ARGUMENT_NULL_IF_NULL(target_leds->leds);
    
    if(target_leds->count < source_leds_size) {
        THROW(
            ERROR_CODE_INSUFFICIENT_BUFFER,
            "Unable to map from source to target: the target buffer of size %i is by %i too small.",
            target_leds->count,
            source_leds_size - target_leds->count
        );
    }

    for(uint8_t i = 0; i < target_leds->count; i++) {
        map_rgb_ledc_led__to__rgb_led_domain(
            &source_leds[i],
            &target_leds->leds[i]
        );
    }
}