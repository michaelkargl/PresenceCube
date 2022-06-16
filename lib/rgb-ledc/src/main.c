#include "logger.h"
#include "esp_http_server.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rgb_ledc.h"
#include "rgb_ledc_init.h"
#include "rgb_ledc_init_types.h"
#include "esp_err.h"

static const char *_LOGGING_TAG = "main";

// Macros prefixed with CONFIG_ are set by meunconfig config manager
// TODO: move configuration into separate header file
#define LED_PIN_RED 25
#define LED_PIN_GREEN 32
#define LED_PIN_BLUE 33


static void _delay_ms(int milliseconds) { 
    vTaskDelay(milliseconds / portTICK_PERIOD_MS);
}

/**
 * @brief application entry point
 * @return esp_err_t 
 */
esp_err_t app_main()
{
    const ledc_timer_config_t low_speed_ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // 13bit = 2^13 = 8192 levels within 1 cycle
        .freq_hz = 5000,                      // 5kHz = 1 cycle lasts 1/5000s
        .speed_mode = LEDC_LOW_SPEED_MODE,   // Note: ESP32-S2 only supports “low speed” channels
        .timer_num = LEDC_TIMER_0,            // PWM timer
        .clk_cfg = LEDC_AUTO_CLK              // Auto select the source clock
    };

    const struct ledc_rgb_gpio_pins_t pins = {
        .red = LED_PIN_RED,
        .green = LED_PIN_GREEN,
        .blue = LED_PIN_BLUE
    };

    const struct ledc_rgb_channels_t channels = {
        .red = LEDC_CHANNEL_0,
        .green = LEDC_CHANNEL_1,
        .blue = LEDC_CHANNEL_2
    };

    const uint16_t switch_interval_ms = 10000;

    struct ledc_rgb_led_t led = new_rgb_ledc_led(
        "west",
        low_speed_ledc_timer, 
        channels, 
        pins, 
        true,
        switch_interval_ms
    );

    while ( true ) {
        log_information(_LOGGING_TAG, "Switching led colors to red\n");
        set_led_color_percent(&led, 100, 0, 0);
        _delay_ms(switch_interval_ms);
        
        log_information(_LOGGING_TAG, "Switching led colors to green\n");
        set_led_color_percent(&led, 0, 100, 0);
        _delay_ms(switch_interval_ms);
        
        log_information(_LOGGING_TAG, "Switching led colors to blue\n");
        set_led_color_percent(&led, 0, 0, 100);
        _delay_ms(switch_interval_ms);
    }
    
    return ESP_OK;
}