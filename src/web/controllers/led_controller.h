#pragma once
#include "rgb_ledc_types.h"
#include "esp_http_server.h"


struct led_controller_endpoints_t
{
    const httpd_uri_t *get_hello;
    const httpd_uri_t *get_leds;
};

void initialize_led_controller(const struct ledc_rgb_led_t *led);
const struct led_controller_endpoints_t *get_led_controller_endpoints();
