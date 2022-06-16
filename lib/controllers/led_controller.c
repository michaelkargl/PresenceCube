#include "led_controller.h"
#include "webserver.h"
#include "rgb_ledc.h"
#include "delay_service.h"
#include "cJSON.h"
#include "stdbool.h"
#include "info_hud.h"


// TODO: move configuration into a separate configuration header
#define REQUEST_BODY_BUFFER_SIZE 50
#define REQUEST_ERROR_MESSAGE_BUFFER_SIZE 50

static const struct ledc_rgb_led_t *_leds;
static int _leds_size;

static esp_err_t POST_led_handler(httpd_req_t *req);
static esp_err_t GET_leds_handler(httpd_req_t *req);
static esp_err_t OPTIONS_handler(httpd_req_t *req);

static const httpd_uri_t _home_controller_endpoints[] = {
    {.uri = "/led",
     .method = HTTP_POST,
     .handler = POST_led_handler},
    {.uri = "/leds",
     .method = HTTP_GET,
     .handler = GET_leds_handler},
    {.uri = "/led",
     .method = HTTP_OPTIONS,
     .handler = OPTIONS_handler}};

void initialize_led_controller(const struct ledc_rgb_led_t *leds, int leds_size)
{
    _leds = leds;
    _leds_size = leds_size;
}

const httpd_uri_t *get_led_controller_endpoints()
{
    return _home_controller_endpoints;
}

uint8_t get_led_controller_endpoint_count()
{
    return (uint8_t)(sizeof(_home_controller_endpoints) / sizeof(_home_controller_endpoints[0]));
}

static void _send_response(httpd_req_t *request, char *status_code, char *payload)
{
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Allow-Origin", "*"));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Max-Age", "600"));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS"));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Allow-Headers", "*"));
    ESP_ERROR_CHECK(httpd_resp_set_status(request, status_code));
    ESP_ERROR_CHECK(httpd_resp_send(request, payload, HTTPD_RESP_USE_STRLEN));
}

static esp_err_t OPTIONS_handler(httpd_req_t *request)
{
    _send_response(request, HTTPD_204, "Cors approved ;)");
    return ESP_OK;
}

static esp_err_t GET_leds_handler(httpd_req_t *request)
{
    // TODO: move the querying of information into query handler module
    // https://stackoverflow.com/questions/45709054/create-json-object-using-cjson-h
    cJSON *led_array = cJSON_CreateArray();
    cJSON *element;

    // moves the response body building into separate module
    for (int i = 0; i < _leds_size; i++)
    {
        cJSON_AddItemToArray(led_array, element = cJSON_CreateObject());
        cJSON_AddNumberToObject(element, "id", i);
        cJSON_AddStringToObject(element, "name", _leds[i].name);
        cJSON_AddBoolToObject(element, "isInitialized", _leds[i].is_initialized);
        cJSON_AddBoolToObject(element, "isCommonAnode", _leds[i].is_common_anode);
    }

    char *response_string = cJSON_Print(led_array);
    _send_response(request, HTTPD_200, response_string);

    // one has to free the string manually
    free(response_string);
    // free all objects under root and root itself
    cJSON_Delete(led_array);
    led_array = element = NULL;

    return ESP_OK;
}

static esp_err_t POST_led_handler(httpd_req_t *request)
{
    char body_buffer[REQUEST_BODY_BUFFER_SIZE];
    char error_buffer[REQUEST_ERROR_MESSAGE_BUFFER_SIZE];
    esp_err_t status_code = ESP_OK;

    // TODO: move the processing of incoming request text to dto into separate module
    cJSON *json = webserver_try_get_json_request(
        request,
        body_buffer, sizeof(body_buffer) / sizeof(body_buffer[0]),
        error_buffer, sizeof(error_buffer) / sizeof(error_buffer[0]));

    if (json == NULL)
    {
        _send_response(request, HTTPD_500, error_buffer);
        status_code = ESP_FAIL;
        goto end;
    }

    if (!cJSON_HasObjectItem(json, "id"))
    {
        _send_response(request, HTTPD_500, "Json did not contain a led 'id'");
        status_code = ESP_FAIL;
        goto end;
    }

    cJSON *id = cJSON_GetObjectItemCaseSensitive(json, "id");
    bool is_valid_led_id = cJSON_IsNumber(id) && id->valueint >= 0 && id->valueint <= (_leds_size - 1);
    if (!is_valid_led_id)
    {
        _send_response(request, HTTPD_500, "Led 'id' is not a valid number => see /leds");
        status_code = ESP_FAIL;
        goto end;
    }

    cJSON *red = cJSON_GetObjectItemCaseSensitive(json, "r");
    cJSON *green = cJSON_GetObjectItemCaseSensitive(json, "g");
    cJSON *blue = cJSON_GetObjectItemCaseSensitive(json, "b");
    bool isValidRgb = cJSON_IsNumber(red) && red->valueint >= 0 && cJSON_IsNumber(green) && green->valueint >= 0 && cJSON_IsNumber(blue) && blue->valueint >= 0;
    if (!isValidRgb)
    {
        _send_response(request, HTTPD_500, "Json string does not provide r,g,b percent values. Example: {\"r\":0,\"g\":100,\"b\":0}.");
        status_code = ESP_FAIL;
        goto end;
    }


    // TODO: move the issuing of logic into command handler
    set_led_color_8bit(
        &_leds[id->valueint],
        red->valueint,
        green->valueint,
        blue->valueint);

    _send_response(request, HTTPD_200, "OK");

end:
    cJSON_Delete(json);
    return status_code;
}
