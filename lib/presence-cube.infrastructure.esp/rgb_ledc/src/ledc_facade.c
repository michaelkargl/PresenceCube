#include "ledc_facade.h"
#include "esp_err.h"
#include "driver/ledc.h"
#include "ledc_led_duty_calculator.h"
#include "ledc_led_t.h"
#include "CException.h"

#define TAG "ledc.c"

typedef esp_err_t (*ledc_set_duty_func_t)(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t target_duty);
typedef esp_err_t (*ledc_update_duty_func_t)(ledc_mode_t speed_mode, ledc_channel_t channel);
typedef esp_err_t (*ledc_set_fade_with_time_func_t)(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t target_duty, int max_fade_time_ms);
typedef esp_err_t (*ledc_fade_start_func_t)(ledc_mode_t speed_mode, ledc_channel_t channel, ledc_fade_mode_t fade_mode);

ledc_set_duty_func_t _ledc_facade__ledc_set_duty = ledc_set_duty;
ledc_update_duty_func_t _ledc_facade__ledc_update_duty = ledc_update_duty;
ledc_set_fade_with_time_func_t _ledc_facade__ledc_set_fade_with_time  = ledc_set_fade_with_time;
ledc_fade_start_func_t _ledc_facade__ledc_fade_start = ledc_fade_start;

static void _update_duty_hard(const ledc_channel_config_t *channel, uint32_t duty)
{
    THROW_ARGUMENT_NULL_IF_NULL(channel);

    log_debug(TAG, "Switching duty cycle of channel %d from %d => %d\n", channel->channel, channel->duty, duty);
    ESP_ERROR_CHECK(_ledc_facade__ledc_set_duty(channel->speed_mode, channel->channel, duty));
    ESP_ERROR_CHECK(_ledc_facade__ledc_update_duty(channel->speed_mode, channel->channel));
}

static void _update_duty_softly(const ledc_channel_config_t *channel, uint32_t duty, int fade_time_ms)
{
    THROW_ARGUMENT_NULL_IF_NULL(channel);

    log_information(TAG, "Softly setting duty cycle of channel %d from %d => %i over %d ms\n",
                    channel->channel,
                    channel->duty,
                    duty,
                    fade_time_ms);

    ESP_ERROR_CHECK(_ledc_facade__ledc_set_fade_with_time(channel->speed_mode, channel->channel, duty, fade_time_ms));
    ESP_ERROR_CHECK(_ledc_facade__ledc_fade_start(channel->speed_mode, channel->channel, LEDC_FADE_NO_WAIT));
}

static void _update_duty(const ledc_channel_config_t *channel, uint32_t duty, int fade_time_ms)
{
    THROW_ARGUMENT_NULL_IF_NULL(channel);

    if (fade_time_ms > 0)
    {
        _update_duty_softly(channel, duty, fade_time_ms);
    }
    else
    {
        _update_duty_hard(channel, duty);
    }
}

void ledc_set_percent(const struct ledc_led_t *led, uint8_t percent, int fade_time_ms)
{
    THROW_ARGUMENT_NULL_IF_NULL(led);
    THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT_REF(led);

    uint32_t duty_cycle = calculate_duty_cycle(led, percent);
    _update_duty(&led->channel, duty_cycle, fade_time_ms);
}
