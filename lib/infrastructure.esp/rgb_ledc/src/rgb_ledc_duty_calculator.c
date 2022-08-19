#include "rgb_ledc_duty_calculator.h"
#include "logger.h"
#include "math.h"
#include "math_util.h"
#include "CException.h"

static const char* MODULE_NAME = "rgb_ledc_duty_calculator";

static uint32_t _calculate_max_duty(uint32_t bit_resolution) {
    return pow(2, bit_resolution);
}

static uint32_t _calculate_duty_cycle(uint32_t bit_resolution, uint8_t percent) {
    percent = ranged_value(percent, 0, 100);

    uint32_t max_duty = _calculate_max_duty(bit_resolution);
    uint32_t duty = max_duty / 100.0f * percent;

    log_information(MODULE_NAME, "Calculated duty %d for percent %d at resolution of %i bit.\n", duty, percent, bit_resolution);
    return duty;
}

uint32_t get_max_duty(const struct ledc_led_t *led)
{
    if ( led == NULL ) {
        Throw(ERROR_CODE_ARGUMENT_NULL);
    }

    if ( !led->is_initialized ) {
        Throw(ERROR_CODE_UNINITIALIZED_DATA_ACCESS);
    }
    
    int bit_resolution = (int)led->timer.duty_resolution;
    int duty = _calculate_max_duty(bit_resolution);

    log_information(MODULE_NAME, "Calculated max duty of %d with a resolution of %d\n", duty, bit_resolution);
    return duty;
}

uint32_t calculate_duty(const struct ledc_led_t *led, uint8_t percent)
{
    if ( led == NULL ) {
        Throw(ERROR_CODE_ARGUMENT_NULL);
    }

    if ( !led->is_initialized ) {
        Throw(ERROR_CODE_UNINITIALIZED_DATA_ACCESS);
    }

    percent = ranged_value(percent, 0, 100);
    percent = led->is_common_anode
        ? 100 - percent
        : percent;

    return _calculate_duty_cycle(led->timer.duty_resolution, percent);
}
