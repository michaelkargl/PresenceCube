#include "ledc_rgb_gpio_pins_t.h"
#include "ledc_rgb_channels_t.h"
#include "ledc_rgb_led_t.h"
#include "ledc_led_duty_calculator.h"
#include "rgb_ledc_init.h"
#include "esp_err.h"
#include "logger.h"
#include "string.h"
#include "ledc_init.h"
#include "exception_handling.h"

static const char *TAG = "rgb_ledc_init";


void (*_rgb_ledc_init__configure_led)(const struct ledc_led_t *) = configure_led;


// TODO: find a better suiting name that clarifies that this actually enables the leds for ledc control
void configure_rgb_led(const struct ledc_rgb_led_t *led)
{
    if ( led == NULL ) {
        Throw(ERROR_CODE_ARGUMENT_NULL);
    }

    if (!led->is_initialized) {
        log_error(TAG, "Trying to configure uninitialized RGB LED (address: %p).\n", led);
        Throw(ERROR_CODE_UNINITIALIZED_DATA_ACCESS);
    }

    log_information(TAG, "Configuring RGB LED '%s'\n", led->name);
    _rgb_ledc_init__configure_led(&led->red);
    _rgb_ledc_init__configure_led(&led->green);
    _rgb_ledc_init__configure_led(&led->blue);
}

void configure_rgb_leds(const struct ledc_rgb_led_t *leds, uint8_t led_count) {
    if ( leds == NULL ) {
        Throw(ERROR_CODE_ARGUMENT_NULL);
    }

    for(uint8_t i = 0; i < led_count; i++) {
        configure_rgb_led(leds + i);
    }
}