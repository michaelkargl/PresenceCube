#include "build_ledc_led_mock.h"


static struct ledc_led_t return_value;
static uint8_t call_counter = 0;


void set_mock_return_value(struct ledc_led_t led) {
    return_value = led;
}

void build_ledc_led_mock__reset() {
    call_counter = 0;
}

uint8_t build_ledc_led_mock__get_callcount() {
    return call_counter;
}

struct ledc_led_t build_ledc_led_mock__build_ledc_led(
    char name[10],
    ledc_timer_config_t timer,
    ledc_channel_t channel,
    uint8_t gpio_pin,
    bool is_common_anode
) {
    call_counter++;
    return return_value;
}