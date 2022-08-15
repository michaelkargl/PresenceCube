#include "ledc_rgb_gpio_pins_t.h"
#include "ledc_rgb_channels_t.h"
#include "ledc_rgb_led_t.h"
#include "rgb_ledc_duty_calculator.h"
#include "rgb_ledc_init.h"
#include "esp_err.h"
#include "logger.h"
#include "string.h"
#include "ledc_init.h"

static const char *TAG = "rgb_ledc_init";


// TODO: find a better suiting name that clarifies that this actually enables the leds for ledc control
void configure_rgb_led(const struct ledc_rgb_led_t *led)
{
    if (!led->is_initialized) {
        log_error(TAG, "Trying to configure uninitialized RGB LED (address: %p). Skipping...'\n", led);
        return;
    }

    log_information(TAG, "Configuring RGB LED '%s'\n", led->name);
    configure_led(&led->red);
    configure_led(&led->green);
    configure_led(&led->blue);
}

void configure_rgb_leds(const struct ledc_rgb_led_t *leds, uint8_t led_count) {
    for(uint8_t i = 0; i < led_count; i++) {
        configure_rgb_led(leds + i);
    }
}