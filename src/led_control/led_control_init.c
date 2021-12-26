#include "led_control_init.h"
#include "rgb_ledc.h"

#define RGB_LED_COUNT 2

static const char *TAG = "led_control_init";
static struct ledc_rgb_led_t _leds[RGB_LED_COUNT];


static struct ledc_rgb_led_t _new_rgbled(
    ledc_timer_config_t _ledc_timer,
    const struct ledc_rgb_channels_t channels,
    const struct ledc_rgb_gpio_pins_t pins,
    bool is_common_anode,
    int32_t fade_interval
)
{
    struct ledc_rgb_led_t led = {
        .name = "southwest",
        .is_initialized = true,
        .is_common_anode = is_common_anode,
        .fade_milliseconds = fade_interval,
        .red = {
            .name = "red",
            .timer = _ledc_timer,
            .channel = { 
                .channel = channels.red,
                .duty = is_common_anode ? get_max_duty(&_ledc_timer) : 0,
                .gpio_num = pins.red,
                .speed_mode = _ledc_timer.speed_mode,
                .hpoint = 0,
                .timer_sel = _ledc_timer.timer_num,
                .intr_type = LEDC_INTR_DISABLE
            },
        },
        .green = {
            .name = "green",
            .timer = _ledc_timer,
            .channel = { 
                .channel = channels.green,
                .duty = is_common_anode ? get_max_duty(&_ledc_timer) : 0,
                .gpio_num = pins.green,
                .speed_mode = _ledc_timer.speed_mode,
                .hpoint = 0,
                .timer_sel = _ledc_timer.timer_num,
                .intr_type = LEDC_INTR_DISABLE
            },
        },
        .blue = {
            .name = "blue",
            .timer = _ledc_timer,
            .channel = { 
            .channel = channels.blue,
                .duty = is_common_anode ? get_max_duty(&_ledc_timer) : 0,
                .gpio_num = pins.blue,
                .speed_mode = _ledc_timer.speed_mode,
                .hpoint = 0,
                .timer_sel = _ledc_timer.timer_num,
                .intr_type = LEDC_INTR_DISABLE
            },
        }};
    
    configure_rgb_led(&led);

    return led;
}

esp_err_t initialize_led_control() {
    const ledc_timer_config_t _low_speed_ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // 13bit = 2^13 = 8192 levels within 1 cycle
        .freq_hz = 5000,                      // 5kHz = 1 cycle lasts 1/5000s
        .speed_mode = LEDC_LOW_SPEED_MODE,   // Note: ESP32-S2 only supports “low speed” channels
        .timer_num = LEDC_TIMER_0,            // PWM timer
        .clk_cfg = LEDC_AUTO_CLK              // Auto select the source clock
    };

    const struct ledc_rgb_gpio_pins_t _pins = {
        .red = CONFIG_RGB_RED_CHANNEL_PIN,
        .green = CONFIG_RGB_GREEN_CHANNEL_PIN,
        .blue = CONFIG_RGB_BLUE_CHANNEL_PIN
    };

    const struct ledc_rgb_channels_t _channels = {
        .red = LEDC_CHANNEL_0,
        .green = LEDC_CHANNEL_1,
        .blue = LEDC_CHANNEL_2
    };

    _leds[0] = _new_rgbled(_low_speed_ledc_timer, _channels, _pins, true, 1000);

    return ESP_OK;
}

const struct ledc_rgb_led_t* get_led_control_leds() {
    return _leds;
}

int get_led_control_initialized_led_count() {
    int initialized_led_count = 0;
    for ( int i = 0; i < RGB_LED_COUNT; i++ ) {
        if (_leds[i].is_initialized) {
            initialized_led_count++;
        }
    }
    return initialized_led_count;
}