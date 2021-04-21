#pragma once
#include "mqtt_handlers.h"
#include "mqtt_client.h"

esp_mqtt_client_handle_t setup_mqtt_connection(
    const char* uri,
    const char* username,
    const char* password);
void register_mqtt_event_handlers(mqtt_handlers_t event_handlers);

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