#include "ledc_init.h"
#include "driver/ledc.h"
#include "logger.h"
#include "CException.h"

#define TAG "ledc_init"

esp_err_t (*_ledc_init__ledc_timer_config)(const ledc_timer_config_t* timer_configuration) = ledc_timer_config;
esp_err_t (*_ledc_init__ledc_channel_config)(const ledc_channel_config_t* ledc_configuration) = ledc_channel_config;
esp_err_t (*_led_init__ledc_fade_func_install)(int interrupt_allocation_flags) = ledc_fade_func_install;


void configure_led(const struct ledc_led_t *led)
{
    if (!led->is_initialized) {
        log_error(TAG, "Trying to configure uninitialized LED (address: %p).\n", led);
        Throw(UNINITIALIZED_DATA_ACCESS_ERROR);
    }
    
    log_information(TAG, "Configuring LED '%s'\n", led->name);
    ESP_ERROR_CHECK(_ledc_init__ledc_timer_config(&led->timer));

    log_information(TAG, "Configuring LED Channel %i\n", led->channel.intr_type);
    ESP_ERROR_CHECK(_ledc_init__ledc_channel_config(&led->channel));

    _led_init__ledc_fade_func_install(0);
}