#include "map_ledc_led__led_domain.h"
#include "ledc_led_t.h"
#include "CException.h"
#include "string.h"
#include "math.h"
#include "ledc_led_duty_calculator.h"
#include "ledc_led_duty_calculator_func.h"

calculate_duty_percent_func_t _map_ledc_led__led_domain__calculate_duty_percent = calculate_duty_percent;

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

void map_ledc_led_array__to__led_domain_array(
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

    for(uint8_t i = 0; i < target_led_count; i++) {
        target_leds[i] = map_ledc_led__to__led_domain(&source_leds[i]);
    }
}