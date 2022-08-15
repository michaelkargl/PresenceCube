#ifndef _LED_CONTROLLER
#define _LED_CONTROLLER

#include "esp_http_server.h"
#include "ledc_rgb_led_t.h"

struct led_controller_endpoints_t
{
    const httpd_uri_t *get_hello;
    const httpd_uri_t *get_leds;
};

void initialize_led_controller(const struct ledc_rgb_led_t *led, int leds_size);
const httpd_uri_t *get_led_controller_endpoints();
uint8_t get_led_controller_endpoint_count();

#endif /* _LED_CONTROLLER */