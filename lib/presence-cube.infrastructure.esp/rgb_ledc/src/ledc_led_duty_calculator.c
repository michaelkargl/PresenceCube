#include "ledc_led_duty_calculator.h"
#include "logger.h"
#include "math.h"
#include "math_util.h"
#include "CException.h"


static const char* MODULE_NAME = "ledc_led_duty_calculator";

static uint32_t _calculate_max_duty(uint32_t bit_resolution) {
    return pow(2, bit_resolution);
}


static uint32_t _calculate_duty_cycle(uint32_t bit_resolution, uint8_t percent) {
    percent = ranged_value(percent, 0, 100);

    uint32_t max_duty = _calculate_max_duty(bit_resolution);
    uint32_t duty = max_duty / 100.0f * percent;

    log_debug(MODULE_NAME, "Calculated duty=%d for percent=%d at resolution=%ibit.\n", duty, percent, bit_resolution);
    return duty;
}

static float _calculate_duty_percentage(uint32_t bit_resolution, uint32_t duty) {
    uint32_t max_duty = _calculate_max_duty(bit_resolution);
    float percent = duty / (float)max_duty * 100;

    log_debug(MODULE_NAME, "Calculated a percentage=%d for duty=%d with resolution=%ibit.\n", duty, percent, bit_resolution);
    return percent;
}



uint32_t calculate_duty_cycle(const struct ledc_led_t *led, uint8_t percent)
{
    THROW_ARGUMENT_NULL_IF_NULL(led);
    THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT_REF(led);

    percent = ranged_value(percent, 0, 100);
    percent = led->is_common_anode
        ? 100 - percent
        : percent;

    return _calculate_duty_cycle(led->timer.duty_resolution, percent);
}

float calculate_duty_percent(const struct ledc_led_t *led) {
    THROW_ARGUMENT_NULL_IF_NULL(led);
    THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT_REF(led);
    
    return _calculate_duty_percentage(
        led->timer.duty_resolution,
        led->channel.duty
    );
}
