#include "wifi_setup.h"


#define ESP_WIFI_SSID CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PASS CONFIG_ESP_WIFI_PASSWORD

static const char *TAG = "toggle_led";


static void wifi_event_handler(
    void *arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data)
{
    ESP_LOGV(TAG, "Wifi event %s raised.", event_base);

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_LOGV(TAG, "Starting Wi-Fi connection...");
        esp_wifi_connect();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGW(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGW(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGW(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}

/**
 * Configures and initializes network interface and eventing
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#wi-fi-lwip-init-phase
*/
void wifi_lwip_init()
{
    // s1.1: creates an LwIP core task and initializes LwIP-related work
    ESP_LOGV(TAG, "Network interface init (netif_init)...");
    ESP_ERROR_CHECK(esp_netif_init());

    // s1.2: creates a system Event task and initialize an application event's callback function
    ESP_LOGV(TAG, "System event loop...");
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // s1.3: Create default network interface, binding station or AP with TCP/IP stack
    ESP_LOGV(TAG, "Creating station...");
    esp_netif_create_default_wifi_sta();

    // s1.4: Configure and create the Wi-Fi-driver
    ESP_LOGV(TAG, "Creating Wi-Fi driver...");
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
}

/**
 * @brief Configures the Wi-Fi driver
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#wi-fi-configuration-phase
 */
void wifi_configuration()
{
    ESP_LOGV(TAG, "Configuring Wi-Fi mode...");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    ESP_LOGV(TAG, "Configuring Wi-Fi...");
    ESP_LOGV(TAG, "   SSID: %s", CONFIG_ESP_WIFI_SSID);
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = ESP_WIFI_SSID,
            .password = ESP_WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        }};
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));

    ESP_LOGV(TAG, "Wiring up application event handlers...");
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &wifi_event_handler,
        NULL,
        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &wifi_event_handler,
        NULL,
        &instance_got_ip));
}

/**
 * @brief starts up the Wi-Fi driver
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#wi-fi-start-phase
 */
void wifi_start()
{
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "wifi_init_sta finished.");
}

void initialize_non_volatile_storage_partition()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

/**
 * @brief Configures and sets up a wi-fi station according to the Wi-Fi programming model
 * @note https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#esp32-wi-fi-programming-model
 */
void create_wifi_station()
{
    initialize_non_volatile_storage_partition();

    wifi_lwip_init();
    wifi_configuration();
    wifi_start();
}