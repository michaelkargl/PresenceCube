#include "mqtt_setup.h"
#include "zube_ledc.h"

// configured using Kconfig.projbuild and idf menuconfig
#define ESP_MQTT_URI CONFIG_ESP_MQTT_BROKER_URI
#define ESP_MQTT_CONTROL_TOPIC CONFIG_ESP_MQTT_CONTROL_TOPIC
#define ESP_MQTT_DATA_TOPIC CONFIG_ESP_MQTT_DATA_TOPIC
#define ESP_MQTT_BROKER_USER CONFIG_ESP_MQTT_BROKER_USER
#define ESP_MQTT_BROKER_PASSWORD CONFIG_ESP_MQTT_BROKER_PASSWORD
#define LEDC_TRANSITION_INTERVAL CONFIG_PULSE_WIDTH_MODULATION_FADE_INTERVAL

static const char *TAG = "zube.mqtt_setup";
static ledc_channel_config_t ledc_channel[3];
void (*mqtt_message_handler)(char *);

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    handle_mqtt_event(event_data);
}

void setup_mqtt_connection(ledc_channel_config_t *ledc_channels, void (*message_handler)(char *))
{
    mqtt_message_handler = message_handler;

    //TODO: Find better way to assign the channels or link zube with mqtt (c++ classes maybe?)
    //memcpy(&ledc_channel, &ledc_channels, sizeof(ledc_channels)); // doesnt work as expected
    ledc_channel[0] = ledc_channels[0];
    ledc_channel[1] = ledc_channels[1];
    ledc_channel[2] = ledc_channels[2];

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

    esp_mqtt_client_handle_t client = event->client;
    esp_mqtt_client_publish(client, ESP_MQTT_DATA_TOPIC, "MQTT_EVENT_CONNECTED", 0, 1, 0);
    esp_mqtt_client_subscribe(client, ESP_MQTT_CONTROL_TOPIC, 0);
}

void mqtt_disconnected_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGD(TAG, "MQTT_EVENT_DISCONNECTED");
    esp_mqtt_client_handle_t client = event->client;
    esp_mqtt_client_publish(client, ESP_MQTT_DATA_TOPIC, "MQTT_EVENT_DISCONNECTED", 0, 1, 0);
}

void mqtt_subscribed_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGD(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
    esp_mqtt_client_handle_t client = event->client;
    esp_mqtt_client_publish(client, ESP_MQTT_DATA_TOPIC, "MQTT_EVENT_SUBSCRIBED", 0, 1, 0);
}

void mqtt_unsubscribed_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
    esp_mqtt_client_publish(client, ESP_MQTT_DATA_TOPIC, "MQTT_EVENT_UNSUBSCRIBED", 0, 1, 0);
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
    esp_mqtt_client_handle_t client = event->client;
    esp_mqtt_client_publish(client, ESP_MQTT_DATA_TOPIC, "OTHER EVENT", 0, 1, 0);
}

void mqtt_data_handler(esp_mqtt_event_handle_t event)
{
    ESP_LOGI(TAG, "MQTT_EVENT_DATA");
    ESP_LOGD(TAG, "TOPIC=%.*s\r\n", event->topic_len, event->topic);
    ESP_LOGD(TAG, "DATA=%.*s\r\n", event->data_len, event->data);

    char *data = event->data;
    mqtt_message_handler(data);
    
    // TODO: use functions to parse request and find right handler
    // TODO: send status / error code to gateway
    // TODO: use senml library to parse request

    cJSON *root = cJSON_Parse(data);

    int packSize = cJSON_GetArraySize(root);
    for (int i = 0; i < packSize; i++)
    {
        cJSON *senmlRecord = cJSON_GetArrayItem(root, i);

        char *actuator_name = cJSON_GetObjectItem(senmlRecord, "bn")->valuestring;
        if (strcmp(actuator_name, "led_rgb") == 0)
        {
            //TODO: Error handling
            char *rgbValueJson = cJSON_GetObjectItem(senmlRecord, "vs")->valuestring;
            cJSON *rgbValues = cJSON_Parse(rgbValueJson);

            int r = max(cJSON_GetObjectItem(rgbValues, "r")->valueint, 255);
            int g = max(cJSON_GetObjectItem(rgbValues, "g")->valueint, 255);
            int b = max(cJSON_GetObjectItem(rgbValues, "b")->valueint, 255);
            ESP_LOGD(TAG, "Received request to set led-color to %i,%i,%i", r, g, b);

            float r_percent = floor((100 / (float)255) * r);
            float g_percent = floor((100 / (float)255) * g);
            float b_percent = floor((100 / (float)255) * b);

            cJSON_Delete(rgbValues); // dispose

            ESP_LOGD(TAG, "Received request to set led-color to %f,%f,%f", r_percent, g_percent, b_percent);
            set_led_color_percent(
                ledc_channel,
                r_percent,
                g_percent,
                b_percent,
                LEDC_TRANSITION_INTERVAL);
        }

        // free memory
        cJSON_Delete(senmlRecord);
    }
}