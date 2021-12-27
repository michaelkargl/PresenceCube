#pragma once
#include "mqtt_handlers.h"
#include "mqtt_client.h"

mqtt_handlers_t initialize_handlers(
    esp_mqtt_client_handle_t client,
    char* mqtt_control_topic,
    char* mqtt_data_topic
);