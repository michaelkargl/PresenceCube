#include "wifi_setup.h"
#include "wifi_init.h"
#include "nvs_init.h"

#define WIFI_SSID CONFIG_ESP_WIFI_SSID
#define WIFI_PASS CONFIG_ESP_WIFI_PASSWORD

static const char *_LOGGING_TAG = "zube.wifi_setup";

/**
 * @brief Configures and sets up a wi-fi station according to the Wi-Fi programming model
 * @note https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#esp32-wi-fi-programming-model
 */
void create_wifi_station()
{
    ESP_LOGI(_LOGGING_TAG, "Initializing non-volatile storage...");
    initialize_non_volatile_storage();

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS},
    };

    ESP_LOGI(_LOGGING_TAG, "Initializing wifi station...");
    init_tcpip_networking(&wifi_config);
}