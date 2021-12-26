#include "webserver_init.h"
#include "webserver.h"
#include "controllers/home_controller.h"
#include "esp_log.h"

static const char *_LOGGING_TAG = "webserver_init";

static esp_err_t _register_endpoints() {
    const struct home_controller_endpoints_t *home_controller_routes = get_home_controller_endpoints();
    
    assert(home_controller_routes);
    
    ESP_ERROR_CHECK(register_endpoint(home_controller_routes->get_hello));
    return ESP_OK;
}

esp_err_t initialize_webserver() {
    ESP_LOGI(_LOGGING_TAG, "Starting webserver...");
    ESP_ERROR_CHECK(start_webserver());

    ESP_LOGI(_LOGGING_TAG, "Registiering endpoints...");
    ESP_ERROR_CHECK(_register_endpoints());

    return ESP_OK;
}