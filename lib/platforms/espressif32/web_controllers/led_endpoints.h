#pragma once

#include "endpoint_base.h"

struct led_controller_endpoints_t
{
    const httpd_uri_t *get_hello;
    const httpd_uri_t *get_leds;
};

const httpd_uri_t *get_led_controller_endpoints();
uint8_t get_led_controller_endpoint_count();