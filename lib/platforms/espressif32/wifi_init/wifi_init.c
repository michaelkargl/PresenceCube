#include "wifi_init.h"
#include "logger.iface.h"
#include "esp_wifi.h"
#include "esp_netif.h"

static const char *LOGGING_TAG = "wifi";

/* FreeRTOS event group to signal when we are connected 
   The event group allows multiple bits for each event
   we only care about two of them 00 and 01
*/
static EventGroupHandle_t wifi_event_group;
static EventBits_t wifi_connection_state;
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

/**
    @see xEventGroupCreate:             https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#_CPPv417xEventGroupCreatev
    @see esp_event_loop_create_default: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_event.html?highlight=esp_event_loop_create_default
*/
static esp_err_t setup_wifi_event_loop()
{
    wifi_event_group = xEventGroupCreate();
    if (wifi_event_group == NULL)
    {
        log_error(LOGGING_TAG, "Failed to setup event groups. Insufficient memory.\n");
        return ESP_ERR_NO_MEM;
    }

    // Create eventloop for wifi_event_group
    return esp_event_loop_create_default();
}

/**
 * @brief Event driven wifi init state machine.
 *       Intentionally not split up into multiple functions to keep reactions at one place for educational reasons 
 * @param arg 
 * @param event_base 
 * @param event_id 
 * @param event_data 
 */
static void handle_wifi_event(
    void *arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data)
{
    if (event_base == WIFI_EVENT)
    {
        log_debug(LOGGING_TAG, "Received WIFI_EVENT event_id %i\n", event_id);
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
        {
            log_information(LOGGING_TAG, "Station initialization started. Connecting...\n");
            esp_wifi_connect();
            break;
        }
        case WIFI_EVENT_STA_DISCONNECTED:
        {
            log_information(LOGGING_TAG, "Station disconnected...\n");
            xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
            break;
        }
        case WIFI_EVENT_STA_CONNECTED:
        {
            log_information(LOGGING_TAG, "WIFI station connected!\n");
            break;
        }
        default:
        {
            log_debug(LOGGING_TAG, "Received event_id %i\n", event_id);
            log_debug(LOGGING_TAG, "Unable to react. Not implemented.\n");
            break;
        }
        }
    }
    else if (event_base == IP_EVENT)
    {
        log_debug(LOGGING_TAG, "Received IP_EVENT\n");
        switch (event_id)
        {
        case IP_EVENT_STA_GOT_IP:
        {
            log_information(LOGGING_TAG, "Received station address.\n");
            ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
            log_information(LOGGING_TAG, "IP:" IPSTR "\n", IP2STR(&event->ip_info.ip));

            xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
            break;
        }
        default:
        {
            log_debug(LOGGING_TAG, "Received event_id %i\n", event_id);
            log_debug(LOGGING_TAG, "Unable to react. Not implemented.\n");
            break;
        }
        }
    }
}

static void register_wifi_callbacks(struct _wifi_handlers_t *wifi_handlers)
{
    log_information(LOGGING_TAG, "Registering wifi handlers\n");

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &handle_wifi_event,
        NULL,
        &wifi_handlers->any_id));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &handle_wifi_event,
        NULL,
        &wifi_handlers->got_ip));

    log_information(LOGGING_TAG, "AnyId wifi handler => %p\n", wifi_handlers->any_id);
    log_information(LOGGING_TAG, "GotIP wifi handler => %p\n", wifi_handlers->got_ip);
}

static void unregister_wifi_callbacks(struct _wifi_handlers_t *wifi_handlers)
{
    log_information(LOGGING_TAG, "Unregistering wifi handlers\n");
    log_debug(LOGGING_TAG, "AnyId wifi handler => %p\n", wifi_handlers->any_id);
    log_debug(LOGGING_TAG, "GotIP wifi handler => %p\n", wifi_handlers->got_ip);

    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_handlers->got_ip));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_handlers->any_id));
}

static EventBits_t await_tcpip_connection()
{
    log_debug(LOGGING_TAG, "Awaiting TCP/IP connection.\n");

    // Waiting until either the connection is established or failed.
    // Event bits are set from the wifi event state machine (see above)
    return xEventGroupWaitBits(
        wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);
}

/**
 * @see WIFI_INIT_CONFIG_DEFAULT: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html?highlight=wifi_init_config_default#_CPPv413esp_wifi_initPK18wifi_init_config_t
 * @see esp_wifi_init: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html?highlight=wifi_init_config_default#_CPPv413esp_wifi_initPK18wifi_init_config_t
 * @see Protected Management Frame: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi-security.html?highlight=pmf_cfg#protected-management-frames-pmf
 */
esp_err_t init_tcpip_networking(wifi_config_t *wifi_config)
{
    ESP_ERROR_CHECK(setup_wifi_event_loop());
    // initialize the underlying TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());
    // create objects for wifi station mode
    esp_netif_create_default_wifi_sta();

    // This API must be called before all other WiFi API can be called
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    struct _wifi_handlers_t wifi_handlers = {
        .any_id = NULL,
        .got_ip = NULL};
    register_wifi_callbacks(&wifi_handlers);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    log_information(LOGGING_TAG, "wifi_init_sta finished.\n");

    wifi_connection_state = await_tcpip_connection();

    // clean up and free space
    unregister_wifi_callbacks(&wifi_handlers);
    vEventGroupDelete(wifi_event_group);

    return ESP_OK;
}

bool is_wifi_connected()
{
    return wifi_connection_state & WIFI_CONNECTED_BIT;
}

bool is_wifi_connection_failed()
{
    return wifi_connection_state & WIFI_FAIL_BIT;
}
