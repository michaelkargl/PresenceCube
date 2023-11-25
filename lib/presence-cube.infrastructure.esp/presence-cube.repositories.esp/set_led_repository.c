#include "set_led_repository.h"
#include "ledc_led_duty_calculator.h"
#include "ledc_rgb_led_t.h"
#include "led_store.h"
#include "throwing_helpers.h"

static led_store__get_led_func_t led_store__get_led_fn = led_store__get_led;
static calculate_duty_cycle_func_t ledc_led_duty_calculator__calculate_duty_cycle_fn = ledc_led_duty_calculator__calculate_duty_cycle;

void set_led_repository__update(uint8_t id, uint8_t red, uint8_t green, uint8_t blue)
{
    struct ledc_rgb_led_t *led = led_store__get_led_fn(id);
    THROW_RESOURCE_NOT_FOUND_IF_NULL(led, "A led with id=%u could not be found.", id);

    led->red.channel.duty = ledc_led_duty_calculator__calculate_duty_cycle_fn(&led->red, red);
    led->green.channel.duty = ledc_led_duty_calculator__calculate_duty_cycle_fn(&led->green, green);
    led->blue.channel.duty = ledc_led_duty_calculator__calculate_duty_cycle_fn(&led->blue, blue);

    led_store__update(*led);
}