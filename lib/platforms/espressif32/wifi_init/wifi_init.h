#include "esp_netif.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

struct _wifi_handlers_t
{
    esp_event_handler_instance_t any_id;
    esp_event_handler_instance_t got_ip;
};

esp_err_t init_tcpip_networking(wifi_config_t *wifi_config);
bool is_wifi_connected();
bool is_wifi_connection_failed();