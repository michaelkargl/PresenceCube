#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "mqtt_client.h"
#include "cJSON.h"
#include "string.h"

// configured using Kconfig.projbuild and idf menuconfig
#define RED_LED_PIN CONFIG_RED_LED_PIN
#define GREEN_LED_PIN CONFIG_GREEN_LED_PIN
#define YELLOW_LED_PIN CONFIG_YELLOW_LED_PIN

#define ESP_WIFI_SSID CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PASS CONFIG_ESP_WIFI_PASSWORD
#define ESP_MQTT_URI CONFIG_ESP_MQTT_BROKER_URI
#define ESP_MQTT_CONTROL_TOPIC CONFIG_ESP_MQTT_CONTROL_TOPIC
#define ESP_MQTT_DATA_TOPIC CONFIG_ESP_MQTT_DATA_TOPIC
#define ESP_MQTT_BROKER_USER CONFIG_ESP_MQTT_BROKER_USER
#define ESP_MQTT_BROKER_PASSWORD CONFIG_ESP_MQTT_BROKER_PASSWORD

static const char *TAG = "toggle_led";

/**
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/log.html#logging-library
 */
void configure_logging()
{
    // By default log only messages with levels <= warnings
    esp_log_level_set("*", ESP_LOG_WARN);

    // enable all log levels for the current module tag
    esp_log_level_set(TAG, ESP_LOG_VERBOSE);
}

void configure_pins()
{
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(RED_LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(GREEN_LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(YELLOW_LED_PIN, GPIO_MODE_OUTPUT);
}

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

/**
 * Sets the level on the connected GPIO pin
 * @param level 0: off, 1: on
 */
void toggle_led(gpio_num_t led_pin, uint32_t level)
{
    ESP_LOGD(TAG, "Setting LED-Level on Pin %d to %d\n", led_pin, level);
    gpio_set_level(led_pin, level);
}

static void handle_mqtt_event(esp_mqtt_event_handle_t event)
{

    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGD(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_publish(client, ESP_MQTT_DATA_TOPIC, "MQTT_EVENT_CONNECTED", 0, 1, 0);
        msg_id = esp_mqtt_client_subscribe(client, ESP_MQTT_CONTROL_TOPIC, 0);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "MQTT_EVENT_DISCONNECTED");
        msg_id = esp_mqtt_client_publish(client, ESP_MQTT_DATA_TOPIC, "MQTT_EVENT_DISCONNECTED", 0, 1, 0);
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGD(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, ESP_MQTT_DATA_TOPIC, "MQTT_EVENT_SUBSCRIBED", 0, 1, 0);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, ESP_MQTT_DATA_TOPIC, "MQTT_EVENT_UNSUBSCRIBED", 0, 1, 0);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        ESP_LOGD(TAG, "TOPIC=%.*s\r\n", event->topic_len, event->topic);
        ESP_LOGD(TAG, "DATA=%.*s\r\n", event->data_len, event->data);
        char *data = event->data;
        cJSON *root = cJSON_Parse(data);

        int packSize = cJSON_GetArraySize(root);
        for (int i = 0; i < packSize; i++)
        {
            cJSON *senmlRecord = cJSON_GetArrayItem(root, i);
            char *actuator_name = cJSON_GetObjectItem(senmlRecord, "bn")->valuestring;
            int actuator_value = cJSON_GetObjectItem(senmlRecord, "v")->valueint;
            ESP_LOGD(TAG, "Requesting LED %s to be set to %d", actuator_name, actuator_value);

            if (strcmp(actuator_name, "led_red") == 0)
            {
                toggle_led(RED_LED_PIN, (uint32_t)actuator_value);
            }
            else if (strcmp(actuator_name, "led_yellow") == 0)
            {
                toggle_led(YELLOW_LED_PIN, (uint32_t)actuator_value);
            }
            else if (strcmp(actuator_name, "led_green") == 0)
            {
                toggle_led(GREEN_LED_PIN, (uint32_t)actuator_value);
            }   
        }

        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        msg_id = esp_mqtt_client_publish(client, ESP_MQTT_DATA_TOPIC, "MQTT_EVENT_ERROR", 0, 1, 0);
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        msg_id = esp_mqtt_client_publish(client, ESP_MQTT_DATA_TOPIC, "OTHER EVENT", 0, 1, 0);
        break;
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    handle_mqtt_event(event_data);
}

void setup_mqtt_connection()
{
    esp_mqtt_client_config_t mqtt_configuration = {
        .uri = ESP_MQTT_URI,
        .username = ESP_MQTT_BROKER_USER,
        .password = ESP_MQTT_BROKER_PASSWORD};

    ESP_LOGV(TAG, "Initializing MQTT client...");
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_configuration);

    ESP_LOGV(TAG, "Wiring up MQTT callbacks...");
    esp_mqtt_client_register_event(
        client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);

    ESP_LOGV(TAG, "Starting MQTT client...");
    esp_mqtt_client_start(client);
}

void app_main(void)
{
    configure_logging();
    configure_pins();

    create_wifi_station();
    setup_mqtt_connection();
}