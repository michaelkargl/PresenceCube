#include "esp_log.h"
#include "driver/ledc.h"
#include "math_helper.h"
#include <math.h>

void set_led_color(ledc_channel_config_t *rgb_channels, int32_t duty_red, int32_t duty_green, int32_t duty_blue, int32_t fade_time_ms);
void set_color_soft(ledc_channel_config_t channel, int32_t duty, int32_t fade_time_ms);
void set_color_hard(ledc_channel_config_t channel, int32_t duty);
void set_led_color_percent(ledc_channel_config_t *channels, int percent_red, int percent_green, int percent_blue, int transition_interval_ms);
int calculate_duty(int8_t percent);
int get_max_duty(int duty_resolution);
void configure_ledc(ledc_channel_config_t *ledc_channels);