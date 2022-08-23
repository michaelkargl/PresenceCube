#include "map_ledc_led__led_domain.h"
#include "ledc_led_t.h"
#include "CException.h"
#include "string.h"
#include "math.h"
#include "ledc_led_duty_calculator.h"
#include "ledc_led_duty_calculator_func.h"


calculate_duty_percent_func_t _map_ledc_led__led_domain__calculate_duty_percent = calculate_duty_percent;


led_domain_t map_ledc_led__to__led_domain(struct ledc_led_t *led) {
    THROW_ARGUMENT_NULL_IF_NULL(led);
    
    led_domain_t target = {
        .is_initialized = led->is_initialized,
        .brightness_percent = _map_ledc_led__led_domain__calculate_duty_percent(led),
        .id = led->id
    };

    strncpy(target.display_name, led->name, sizeof(target.display_name));
    
    return target;
}
