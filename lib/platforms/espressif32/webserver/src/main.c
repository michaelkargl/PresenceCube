#include "stdio.h"
#include "esp_err.h"
#include "webserver.h"
#include "home_controller.h"
#include "esp_wifi.h"
#include "nvs_init.h"
#include "logger.h"
#include "wifi_init.h"

// TODO: move configuration into separate header file
#define WIFI_SSID CONFIG_WIFI_SSID
#define WIFI_PASSWORD CONFIG_WIFI_PASSWORD

static const char *LOGGING_TAG = "main";

esp_err_t app_main()
{   
    printf("Hello Webserver");

    initialize_non_volatile_storage(); // wifi requirement

    // beware, runs out of scope once main
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                // Use protected management frames if access point can handle it
                .capable = true,
                .required = false}}};

    ESP_ERROR_CHECK(init_tcpip_networking(&wifi_config));
    if (is_wifi_connection_failed() || !is_wifi_connected())
    {
        ESP_LOGE(LOGGING_TAG, "Failed to connect to SSID: %s", WIFI_SSID);
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGI(LOGGING_TAG, "Connected to Ssid: %s", CONFIG_WIFI_SSID);
    }

    ESP_ERROR_CHECK(webserver_start());

    const struct home_controller_endpoints_t *home_controller_routes = get_home_controller_endpoints();
    assert(home_controller_routes);
    ESP_ERROR_CHECK(webserver_register_endpoint(home_controller_routes->get_hello));

    return ESP_OK;
}