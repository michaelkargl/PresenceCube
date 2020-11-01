#include "cJSON.h"
#include "mqtt_client.h"
#include "math_helper.h"
#include "esp_log.h"
#include "zube_ledc.h"

void setup_mqtt_connection(ledc_channel_config_t *ledc_channels, void (*message_handler)(char *));

void handle_mqtt_event(esp_mqtt_event_handle_t event);
void mqtt_connected_handler(esp_mqtt_event_handle_t event);
void mqtt_disconnected_handler(esp_mqtt_event_handle_t event);
void mqtt_subscribed_handler(esp_mqtt_event_handle_t event);
void mqtt_unsubscribed_handler(esp_mqtt_event_handle_t event);
void mqtt_published_handler(esp_mqtt_event_handle_t event);
void mqtt_data_handler(esp_mqtt_event_handle_t event);
void mqtt_error_handler(esp_mqtt_event_handle_t event);
void mqtt_any_handler(esp_mqtt_event_handle_t event);
void mqtt_before_connect_handler(esp_mqtt_event_handle_t event);
void mqtt_unknown_handler(esp_mqtt_event_handle_t event);