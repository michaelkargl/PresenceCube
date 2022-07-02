#include "esp_http_server.h"

struct home_controller_endpoints_t
{
    const httpd_uri_t *get_hello;
};

const struct home_controller_endpoints_t *get_home_controller_endpoints();