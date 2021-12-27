#include "esp_http_server.h"

struct led_controller_endpoints_t
{
    const httpd_uri_t *get_hello;
};

const struct led_controller_endpoints_t *get_led_controller_endpoints();