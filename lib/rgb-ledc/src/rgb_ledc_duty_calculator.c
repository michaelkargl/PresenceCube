#include "rgb_ledc_duty_calculator.h"
#include "logger.h"
#include "math.h"
#include "math_util.h"

static const char* MODULE_NAME = "rgb_ledc_duty_calculator";

int get_max_duty(const struct ledc_led_t *led)
{
    int duty_resolution = (int)led->timer.duty_resolution;
    int duty = pow(2, duty_resolution) - 1;

    log_debug(MODULE_NAME, "Calculated max duty of %d with a resolution of %d\n", duty, duty_resolution);
    return duty;
}

int _calculate_duty(const struct ledc_led_t *led, int8_t percent)
{
    int max_duty = get_max_duty(led);
    int duty = floor((max_duty / (float)100) * percent);
    duty = ranged_value(duty, 0, max_duty);

    log_debug(MODULE_NAME, "Calculated duty %d for percent %d\n", duty, percent);
    return duty;
}
