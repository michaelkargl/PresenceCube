#include "led_control_init.h"
#include "rgb_ledc_init_types.h"
#include "rgb_ledc_init.h"
#include "rgb_ledc.h"
#include "driver/ledc.h"
#include "logger.h"
#include "esp_err.h"

#define RGB_LED_COUNT 2

static const char *LOGGER_TAG = "led_control_init";
static struct ledc_rgb_led_t _leds[RGB_LED_COUNT];

static struct ledc_rgb_led_t _new_rgb_led_west()
{
    log_information(LOGGER_TAG, "Initializing LED: West\n");

    const ledc_timer_config_t _low_speed_ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // 13bit = 2^13 = 8192 levels within 1 cycle
        .freq_hz = 5000,                      // 5kHz = 1 cycle lasts 1/5000s
        .speed_mode = LEDC_LOW_SPEED_MODE,    // Note: ESP32-S2 only supports “low speed” channels
        .timer_num = LEDC_TIMER_0,            // PWM timer
        .clk_cfg = LEDC_AUTO_CLK              // Auto select the source clock
    };

    const struct ledc_rgb_gpio_pins_t _pins = {
        // TODO: move configuration into separate header file
        .red = CONFIG_RGBA_WEST_RED_CHANNEL_PIN,
        .green = CONFIG_RGBA_WEST_GREEN_CHANNEL_PIN,
        .blue = CONFIG_RGBA_WEST_BLUE_CHANNEL_PIN};

    const struct ledc_rgb_channels_t _channels = {
        .red = LEDC_CHANNEL_0,
        .green = LEDC_CHANNEL_1,
        .blue = LEDC_CHANNEL_2};

    return new_rgb_ledc_led("west", _low_speed_ledc_timer, _channels, _pins, true, 1000);
}

static struct ledc_rgb_led_t _new_rgb_led_east()
{
    log_information(LOGGER_TAG, "Initializing LED: East\n");

    const ledc_timer_config_t _low_speed_ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // 13bit = 2^13 = 8192 levels within 1 cycle
        .freq_hz = 5000,                      // 5kHz = 1 cycle lasts 1/5000s
        .speed_mode = LEDC_LOW_SPEED_MODE,    // Note: ESP32-S2 only supports “low speed” channels
        .timer_num = LEDC_TIMER_0,            // PWM timer
        .clk_cfg = LEDC_AUTO_CLK              // Auto select the source clock
    };

    const struct ledc_rgb_gpio_pins_t _pins = {
        // TODO: move configuration into separate header file
        .red = CONFIG_RGBA_EAST_RED_CHANNEL_PIN,
        .green = CONFIG_RGBA_EAST_GREEN_CHANNEL_PIN,
        .blue = CONFIG_RGBA_EAST_BLUE_CHANNEL_PIN};

    const struct ledc_rgb_channels_t _channels = {
        .red = LEDC_CHANNEL_3,
        .green = LEDC_CHANNEL_4,
        .blue = LEDC_CHANNEL_5};

    return new_rgb_ledc_led("east", _low_speed_ledc_timer, _channels, _pins, true, 1000);
}

esp_err_t initialize_led_control()
{
    log_information(LOGGER_TAG, "Initializing LEDs for led-control\n");

    _leds[0] = _new_rgb_led_east();
    _leds[1] = _new_rgb_led_west();

    return ESP_OK;
}

const struct ledc_rgb_led_t *get_led_control_leds()
{
    return _leds;
}

int get_led_control_initialized_led_count()
{
    int initialized_led_count = 0;
    for (int i = 0; i < RGB_LED_COUNT; i++)
    {
        if (_leds[i].is_initialized)
        {
            initialized_led_count++;
        }
    }
    return initialized_led_count;
}