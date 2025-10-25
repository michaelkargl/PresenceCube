#ifndef _HUD_CONTROLLER
#define _HUD_CONTROLLER

#include "esp_http_server.h"
#include "esp_err.h"

struct hud_controller_endpoints_t
{
    const httpd_uri_t *post_hud;
};

void initialize_hud_controller();
const httpd_uri_t *get_hud_controller_endpoints();
uint8_t get_hud_controller_endpoint_count();

#endif /* _HUD_CONTROLLER */