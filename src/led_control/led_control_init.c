#include "led_control_init.h"
#include "rgb_ledc_init_types.h"
#include "rgb_ledc_init.h"
#include "rgb_ledc.h"
#include "driver/ledc.h"

#define RGB_LED_COUNT 2

static const char *TAG = "led_control_init";
static struct ledc_rgb_led_t _leds[RGB_LED_COUNT];


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

    _leds[0] = new_rgb_ledc_led("west", _low_speed_ledc_timer, _channels, _pins, true, 1000);

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