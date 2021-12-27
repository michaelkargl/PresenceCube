#include "zube_mqtt_handler.h"
#include "esp_log.h"
#include "cJSON.h"
#include "display.h"

static const char* TAG = "zube_mqtt_handler";
static esp_mqtt_client_handle_t _mqtt_client;
static char* _mqtt_control_topic;
static char* _mqtt_data_topic;

static void _handle_mqtt_connected();
static void _handle_mqtt_mesasge(const char* message);
static void _handle_json_message(cJSON* root);


mqtt_handlers_t initialize_handlers(
    esp_mqtt_client_handle_t client,
    char* mqtt_control_topic,
    char* mqtt_data_topic
) {
    _mqtt_client = client;
    _mqtt_control_topic = mqtt_control_topic;
    _mqtt_data_topic = mqtt_data_topic;

    mqtt_handlers_t _mqtt_handlers = {
        .on_connected = _handle_mqtt_connected,
        .on_message_received = _handle_mqtt_mesasge
    };

    return _mqtt_handlers;
}

static void _handle_mqtt_connected() {
    esp_mqtt_client_subscribe(_mqtt_client, _mqtt_control_topic, 0);
}

static void _handle_mqtt_mesasge(const char *message)
{
    ESP_LOGI(TAG, "=======================================");
    ESP_LOGI(TAG, "Received message: %s.", message);
    ESP_LOGI(TAG, "=======================================");

    // TODO: pass string to command receiver
    cJSON* json_root = cJSON_Parse(message);
    if (json_root != NULL) {
        // TODO: invoke command from json
        _handle_json_message(json_root);
    } else {
        // TODO: invoke warn/err command from json
        ESP_LOGW(TAG, "Expected JSON message but received: %s.", message);
    }

    cJSON_free(json_root);
    //esp_mqtt_client_publish(_mqtt_client, _mqtt_data_topic, message, 0, 1, 0);
}

static void _handle_json_message(cJSON* root) {
    char* json = cJSON_Print(root);
    ESP_LOGI(TAG, "Received JSON message of type %i: %s.", root->type, json);

    if (cJSON_IsString(root)) {
        const rgb565_t foreground = get_rgb565color(0,0,0);
        const rgb565_t background = get_rgb565color(255,255,255);

        // TODO: rename to draw_
        display_random_rectangle();
        display_random_rectangle();
        display_random_rectangle();

        // TODO: Refactor to use char*
        draw_string(30, 160, foreground, background, ((uint8_t*)root->valuestring));
    }
}