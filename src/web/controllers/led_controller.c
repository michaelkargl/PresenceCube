#include "led_controller.h"
#include "esp_log.h"
#include "webserver.h"
#include "rgb_ledc.h"
#include "delay_service.h"
#include "cJSON.h"
#include "stdbool.h"

#define REQUEST_BODY_BUFFER_SIZE 50
#define REQUEST_ERROR_MESSAGE_BUFFER_SIZE 50

static const char *_LOGGER_TAG = "led_controller";
static const struct ledc_rgb_led_t *_leds;
static int _leds_size;

static esp_err_t POST_led_handler(httpd_req_t *req);
static esp_err_t GET_leds_handler(httpd_req_t *req);

static const httpd_uri_t _post_hello = {
    .uri = "/led",
    .method = HTTP_POST,
    .handler = POST_led_handler};

static const httpd_uri_t _get_leds = {
    .uri = "/leds",
    .method = HTTP_GET,
    .handler = GET_leds_handler
};

static const struct led_controller_endpoints_t _home_controller_endpoints = {
    .get_hello = &_post_hello,
    .get_leds = &_get_leds
};

void initialize_led_controller(const struct ledc_rgb_led_t *leds, int leds_size) {
    _leds = leds;
    _leds_size = leds_size;
}

const struct led_controller_endpoints_t *get_led_controller_endpoints()
{
    return &_home_controller_endpoints;
}


static void _send_response(httpd_req_t *request, char* status_code, char* payload) {
    ESP_ERROR_CHECK(httpd_resp_set_status(request, status_code));
    ESP_ERROR_CHECK(httpd_resp_send(request, payload, HTTPD_RESP_USE_STRLEN));
}


static esp_err_t GET_leds_handler(httpd_req_t *request) {
    cJSON* led_array = cJSON_CreateArray();

    for(int i = 0; i < _leds_size; i ++) {
        cJSON* element = cJSON_CreateObject();
        cJSON_AddNumberToObject(element, "id", i);
        cJSON_AddStringToObject(element, "name", _leds[i].name);
        cJSON_AddBoolToObject(element, "isInitialized", _leds[i].is_initialized);
        cJSON_AddBoolToObject(element, "isCommonAnode", _leds[i].is_common_anode);
        cJSON_AddItemToArray(led_array, element);
    }
    
    char* response_string = cJSON_Print(led_array);
    _send_response(request, HTTPD_200, response_string);

    cJSON_free(led_array);
    free(response_string);

    return ESP_OK;
}

static esp_err_t POST_led_handler(httpd_req_t *request)
{
    char body_buffer[REQUEST_BODY_BUFFER_SIZE];
    char error_buffer[REQUEST_ERROR_MESSAGE_BUFFER_SIZE];
    memset(body_buffer, 0, sizeof(body_buffer));
    memset(error_buffer, 0, sizeof(error_buffer));

    cJSON* json = try_get_json_request(
        request,
        body_buffer, sizeof(body_buffer) / sizeof(body_buffer[0]),
        error_buffer, sizeof(error_buffer) / sizeof(error_buffer[0])
    );
    
    if ( json == NULL) {
        _send_response(request, HTTPD_500, error_buffer);
        cJSON_free(json);
        return ESP_FAIL;
    }
    
    if (!cJSON_HasObjectItem(json, "id")) {
        _send_response(request, HTTPD_500, "Json did not contain a led 'id'");
        cJSON_free(json);
        return ESP_FAIL;
    }

    int ledId = cJSON_GetObjectItem(json, "id")->valueint;
    bool is_valid_led_id = ledId >= 0 && ledId <= (_leds_size-1);
    if (!is_valid_led_id) {
        _send_response(request, HTTPD_500, "Led 'id' is not a valid number => see /leds");
        cJSON_free(json);
        return ESP_FAIL;
    }

    cJSON_bool hasRgbValues = cJSON_HasObjectItem(json, "r") 
        && cJSON_HasObjectItem(json, "g")
        && cJSON_HasObjectItem(json, "b");
    
    if (!hasRgbValues) {
        _send_response(request, HTTPD_500, "Json string does not provide r,g,b percent values. Example: {\"r\":0,\"g\":100,\"b\":0}.");
        cJSON_free(json);
        return ESP_FAIL;
    }

    set_led_color_percent(
        &_leds[ledId],
        cJSON_GetObjectItem(json, "r")->valueint,
        cJSON_GetObjectItem(json, "g")->valueint,
        cJSON_GetObjectItem(json, "b")->valueint
    );
    
    cJSON_free(json);
    
    ESP_ERROR_CHECK(httpd_resp_send(request, "hello", HTTPD_RESP_USE_STRLEN));
    ESP_ERROR_CHECK(httpd_resp_send(request, body_buffer, HTTPD_RESP_USE_STRLEN));
    return ESP_OK;
}