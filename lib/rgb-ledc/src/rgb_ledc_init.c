#include "rgb_ledc_init.h"
#include "logger.h"
#include "rgb_ledc_duty_calculator.h"
#include "string.h"
// TODO: replace esp specific error handling with custom logic
#include "esp_err.h"

static const char *TAG = "rgb_ledc_init";


// TODO: replace ledc_led_t with custom dto
// TODO: map to domain object for instances where you need to call external services
void configure_led(const struct ledc_led_t *led)
{
    log_information(TAG, "Configuring LED '%s'\n", led->name);
    ESP_ERROR_CHECK(ledc_timer_config(&led->timer));

    log_information(TAG, "Configuring LED Channel %i\n", led->channel.intr_type);
    ESP_ERROR_CHECK(ledc_channel_config(&led->channel));
}

void configure_rgb_led(const struct ledc_rgb_led_t *led)
{
    log_information(TAG, "Configuring RGB LED '%s'\n", led->name);
    configure_led(&led->red);
    configure_led(&led->green);
    configure_led(&led->blue);

    ledc_fade_func_install(0);
}

static void _turn_off_led(struct ledc_led_t *led, bool is_common_anode) {
    led->channel.duty = is_common_anode ? get_max_duty(led) : 0;
}

static void _turn_off_rgb_led(struct ledc_rgb_led_t *led) {
    _turn_off_led(&led->red, led->is_common_anode);
    _turn_off_led(&led->green, led->is_common_anode);
    _turn_off_led(&led->blue, led->is_common_anode);
}

struct ledc_rgb_led_t new_rgb_ledc_led(
    char name[10],
    ledc_timer_config_t _ledc_timer,
    const struct ledc_rgb_channels_t channels,
    const struct ledc_rgb_gpio_pins_t pins,
    bool is_common_anode,
    int32_t fade_interval
)
{
    struct ledc_led_t led_red = {
        .name = "red",
        .timer = _ledc_timer,
        .channel = { 
            .channel = channels.red,
            .gpio_num = pins.red,
            .speed_mode = _ledc_timer.speed_mode,
            .hpoint = 0,
            .timer_sel = _ledc_timer.timer_num,
            .intr_type = LEDC_INTR_DISABLE
        },
    };

    struct ledc_led_t led_green = {
        .name = "green",
        .timer = _ledc_timer,
        .channel = { 
            .channel = channels.green,
            .gpio_num = pins.green,
            .speed_mode = _ledc_timer.speed_mode,
            .hpoint = 0,
            .timer_sel = _ledc_timer.timer_num,
            .intr_type = LEDC_INTR_DISABLE
        },
    };

    struct ledc_led_t led_blue = {
        .name = "blue",
        .timer = _ledc_timer,
        .channel = { 
        .channel = channels.blue,
            .gpio_num = pins.blue,
            .speed_mode = _ledc_timer.speed_mode,
            .hpoint = 0,
            .timer_sel = _ledc_timer.timer_num,
            .intr_type = LEDC_INTR_DISABLE,
        }
    };


    struct ledc_rgb_led_t rgb_led = {
        .is_initialized = true,
        .is_common_anode = is_common_anode,
        .fade_milliseconds = fade_interval,
        .red = led_red,
        .green = led_green,
        .blue = led_blue
    };


    strncpy(rgb_led.name, name, sizeof(rgb_led.name)/sizeof(rgb_led.name[0]));
    _turn_off_rgb_led(&rgb_led);
    configure_rgb_led(&rgb_led);

    return rgb_led;
}