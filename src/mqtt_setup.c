#include "cJSON.h"
#include "esp_log.h"
#include "mqtt_setup.h"
#include "zube_mqtt_handler.h"

static const char *TAG = "zube.mqtt_setup";
static mqtt_handlers_t _mqtt_handlers;

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    handle_mqtt_event(event_data);
}

void register_mqtt_event_handlers(mqtt_handlers_t event_handlers) {
    _mqtt_handlers = event_handlers;
}

esp_mqtt_client_handle_t setup_mqtt_connection(
    const char* uri,
    const char* username,
    const char* password)
{
    esp_mqtt_client_config_t mqtt_configuration = {
        .uri = uri,
        .username = username,
        .password = password};

    ESP_LOGV(TAG, "Initializing MQTT client...");
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_configuration);

    ESP_LOGV(TAG, "Wiring up MQTT callbacks...");
    esp_mqtt_client_register_event(
        client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);

    ESP_LOGV(TAG, "Starting MQTT client...");
    esp_mqtt_client_start(client);

    return client;
}

// TODO: should event be a pointer?
void handle_mqtt_event(esp_mqtt_event_handle_t event)
{
    // MQTT_EVENT_ERROR = 0,          /*!< on error event, additional context: connection return code, error handle from esp_tls (if supported) */
    // MQTT_EVENT_CONNECTED,      /*!< connected event, additional context: session_present flag */
    // MQTT_EVENT_DISCONNECTED,   /*!< disconnected event */
    // MQTT_EVENT_SUBSCRIBED,     /*!< subscribed event, additional context: msg_id */
    // MQTT_EVENT_UNSUBSCRIBED,   /*!< unsubscribed event */
    // MQTT_EVENT_PUBLISHED,      /*!< published event, additional context:  msg_id */
    // MQTT_EVENT_DATA,           /*!< data event, additional context:
    //                                 - msg_id               message id
    //                                 - topic                pointer to the received topic
    //                                 - topic_len            length of the topic
    //                                 - data                 pointer to the received data
    //                                 - data_len             length of the data for this event
    //                                 - current_data_offset  offset of the current data for this event
    //                                 - total_data_len       total length of the data received
    //                                 Note: Multiple MQTT_EVENT_DATA could be fired for one message, if it is
    //                                 longer than internal buffer. In that case only first event contains topic
    //                                 pointer and length, other contain data only with current data length
    //                                 and current data offset updating.
    //
    // MQTT_EVENT_BEFORE_CONNECT, /*!< The event occurs before connecting */

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        mqtt_connected_handler(event);
        break;
    case MQTT_EVENT_DISCONNECTED:
        mqtt_disconnected_handler(event);
        break;
    case MQTT_EVENT_SUBSCRIBED:
        mqtt_subscribed_handler(event);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        mqtt_unsubscribed_handler(event);
        break;
    case MQTT_EVENT_PUBLISHED:
        mqtt_published_handler(event);
        break;
    case MQTT_EVENT_DATA:
        mqtt_data_handler(event);
        break;
    case MQTT_EVENT_ERROR:
        mqtt_error_handler(event);
        break;
    case MQTT_EVENT_BEFORE_CONNECT:
        mqtt_before_connect_handler(event);
        break;
    default:
        mqtt_unknown_handler(event);
        break;
    }
}

void mqtt_connected_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGD(TAG, "MQTT_EVENT_CONNECTED");
    _mqtt_handlers.on_connected();
}

void mqtt_disconnected_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGD(TAG, "MQTT_EVENT_DISCONNECTED");
}

void mqtt_subscribed_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGD(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
}

void mqtt_unsubscribed_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
}

void mqtt_published_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
}

void mqtt_error_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
}

void mqtt_any_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGI(TAG, "MQTT_EVENT_ANY");
}

void mqtt_before_connect_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGI(TAG, "MQTT_EVENT_BEFORE_CONNECT");
}

void mqtt_unknown_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGI(TAG, "Unknown event type received");
}

void mqtt_data_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGD(TAG, "MQTT_EVENT_DATA");
    ESP_LOGD(TAG, "TOPIC=%.*s\r\n", event->topic_len, event->topic);
    ESP_LOGD(TAG, "DATA=%.*s\r\n", event->data_len, event->data);

    // esp32 mqtt does not clear its buffer after a request
    char subbuff[event->data_len+1];
    memcpy(subbuff, event->data, event->data_len);
    subbuff[event->data_len] = '\0';

    _mqtt_handlers.on_message_received(subbuff);
}