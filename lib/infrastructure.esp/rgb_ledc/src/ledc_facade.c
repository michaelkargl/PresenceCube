#include "ledc_facade.h"
#include "logger.h"
#include "esp_err.h"
#include "driver/ledc.h"
#include "rgb_ledc_duty_calculator.h"
#include "ledc_led_t.h"

#define TAG "ledc.c"


static void _update_duty_hard(const ledc_channel_config_t *channel, uint32_t duty)
{
    // TODO: NULL check

    log_debug(TAG, "Switching duty cycle of channel %d from %d => %d\n", channel->channel, channel->duty, duty);
    
    ESP_ERROR_CHECK(ledc_set_duty(channel->speed_mode, channel->channel, duty));
    ESP_ERROR_CHECK(ledc_update_duty(channel->speed_mode, channel->channel));
}

static void _update_duty_softly(const ledc_channel_config_t *channel, uint32_t duty, int fade_time_ms)
{
    // TODO: NULL check

    log_information(TAG, "Softly setting duty cycle of channel %d from %d => %i over %d ms\n",
             channel->channel, 
             channel->duty,
             duty,
             fade_time_ms);

    ESP_ERROR_CHECK(ledc_set_fade_with_time(channel->speed_mode, channel->channel, duty, fade_time_ms));
    ESP_ERROR_CHECK(ledc_fade_start(channel->speed_mode, channel->channel, LEDC_FADE_NO_WAIT));
}

static void _update_duty(const ledc_channel_config_t *channel, uint32_t duty, int fade_time_ms)
{
    // TODO: NULL check
    // TODO: esp uses int for the fade time => what does a negative fade look like?
    
    if (fade_time_ms > 0)
    {
        _update_duty_softly(channel, duty, fade_time_ms);
    }
    else
    {
        _update_duty_hard(channel, duty);
    }
}


void ledc_set_percent(const struct ledc_led_t *led, uint8_t percent, int fade_time_ms) {
    
    // TODO NULL check
    
    // TODO: TEST

    uint32_t duty_cycle = calculate_duty(led, percent);
    _update_duty(&led->channel, duty_cycle, fade_time_ms);
}
