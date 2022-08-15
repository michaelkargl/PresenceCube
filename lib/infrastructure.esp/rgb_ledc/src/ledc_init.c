#include "ledc_init.h"
#include "driver/ledc.h"
#include "logger.h"

#define TAG "ledc_init"


void configure_led(const struct ledc_led_t *led)
{
    if (!led->is_initialized) {
        log_error(TAG, "Trying to configure uninitialized LED (address: %p). Skipping...'\n", led);
        return;
    }
    
    log_information(TAG, "Configuring LED '%s'\n", led->name);
    ESP_ERROR_CHECK(ledc_timer_config(&led->timer));

    log_information(TAG, "Configuring LED Channel %i\n", led->channel.intr_type);
    ESP_ERROR_CHECK(ledc_channel_config(&led->channel));

    ledc_fade_func_install(0);
}