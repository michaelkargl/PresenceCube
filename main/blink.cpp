#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "number_display.h"

extern "C"
{
#include "zube_ledc.h"
#include "wifi_setup.h"
#include "mqtt_setup.h"
}

// configured using Kconfig.projbuild and idf menuconfig
#define LEDC_TRANSITION_INTERVAL CONFIG_PULSE_WIDTH_MODULATION_FADE_INTERVAL

#define LEDC_HIGH_SPEED_TIMER LEDC_TIMER_0

#define RED_LED_PIN CONFIG_RGB_RED_CHANNEL_PIN
#define GREEN_LED_PIN CONFIG_RGB_GREEN_CHANNEL_PIN
#define BLUE_LED_PIN CONFIG_RGB_BLUE_CHANNEL_PIN

typedef struct
{
    int red;
    int green;
    int blue;
} led_pins;

typedef struct
{
    ledc_channel_t red;
    ledc_channel_t green;
    ledc_channel_t blue;
} led_channels;

ledc_channel_config_t ledc_channel[3];

void setup_rgb_channel(ledc_channel_config_t *ledc_config, int gpio_pin, ledc_channel_t channel)
{
    ledc_config->channel = channel;
    ledc_config->duty = 0;
    ledc_config->gpio_num = gpio_pin;
    ledc_config->speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_config->hpoint = 0;
    ledc_config->timer_sel = LEDC_HIGH_SPEED_TIMER;
}

void setup_rgb_channels(ledc_channel_config_t *ledc_config, led_pins *gpio_pins, led_channels *channels)
{
    setup_rgb_channel(&ledc_config[0], gpio_pins->red, channels->red);
    setup_rgb_channel(&ledc_config[1], gpio_pins->green, channels->green);
    setup_rgb_channel(&ledc_config[2], gpio_pins->blue, channels->blue);
}

void draw_screen(NumberDisplay *display, int number)
{
    auto random = rand() % 100;
    auto filledEllipses = random % 2 == 1;

    display->clearScreen();
    display->displayEllipses(filledEllipses);
    display->displayNumber(number);

    // delay(2000);
}

extern "C" void app_main(void)
{
    led_pins pins = {
        RED_LED_PIN,
        GREEN_LED_PIN,
        BLUE_LED_PIN};
    led_channels channels = {
        LEDC_CHANNEL_1,
        LEDC_CHANNEL_2,
        LEDC_CHANNEL_0};

    NumberDisplay tft_display_service;
    draw_screen(&tft_display_service, 0);

    setup_rgb_channels(ledc_channel, &pins, &channels);
    create_wifi_station();

    configure_ledc(ledc_channel);
    setup_mqtt_connection(ledc_channel);

}
