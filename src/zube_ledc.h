#include "driver/ledc.h"

// TODO: review arguments
void configure_ledc(ledc_channel_config_t *ledc_channels, ledc_timer_config_t ledc_timer);
void set_led_color_percent(
    ledc_channel_config_t *channels, 
    int percent_red, 
    int percent_green, 
    int percent_blue, 
    int transition_interval_ms);
