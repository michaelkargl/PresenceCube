#include "ledc_led_builder.h"
#include "ledc_channel_config_builder.h"
#include "string.h"


static uint8_t id_counter = 0;

static unsigned int _generate_id() {
    return id_counter++;
}


struct ledc_led_t build_ledc_led(
    char name[10],
    ledc_timer_config_t timer,
    ledc_channel_t channel,
    uint8_t gpio_pin,
    bool is_common_anode
) {   
    const ledc_channel_config_t channel_config = build_ledc_channel_config(channel, timer, gpio_pin);
    struct ledc_led_t led = {
        .id = _generate_id(),
        .timer = timer,
        .channel = channel_config,
        .is_common_anode = is_common_anode
    };
    
    strncpy(led.name, name, sizeof(led.name));
    led.is_initialized = true;

    return led;
}
