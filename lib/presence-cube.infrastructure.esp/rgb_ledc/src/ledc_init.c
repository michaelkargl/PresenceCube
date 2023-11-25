#include "ledc_init.h"
#include "driver/ledc.h"
#include "logger.h"
#include "exception_handling.h"

#define TAG "ledc_init"

esp_err_t (*ledc_init__ledc_timer_config)(const ledc_timer_config_t* timer_configuration) = ledc_timer_config;
esp_err_t (*ledc_init__ledc_channel_config)(const ledc_channel_config_t* ledc_configuration) = ledc_channel_config;
esp_err_t (*led_init__ledc_fade_func_install)(int interrupt_allocation_flags) = ledc_fade_func_install;


void configure_led(const struct ledc_led_t *led)
{
    if (led == NULL) {
        Throw(ERROR_CODE_ARGUMENT_NULL);
    }

    if (!led->is_initialized) {
        log_error(TAG, "Trying to configure uninitialized LED (address: %p).\n", led);
        Throw(ERROR_CODE_UNINITIALIZED_DATA_ACCESS);
    }
    
    log_information(TAG, "Configuring LED %u '%s'\n", led->id, led->name);
    ESP_ERROR_CHECK(ledc_init__ledc_timer_config(&led->timer));

    log_information(TAG, "Configuring LED %u Channel %i\n", led->id, led->channel.intr_type);
    ESP_ERROR_CHECK(ledc_init__ledc_channel_config(&led->channel));

    led_init__ledc_fade_func_install(0);
}