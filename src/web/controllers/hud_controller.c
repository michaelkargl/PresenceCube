#include "hud_controller.h"
#include "webserver.h"
#include "display/info_hud.h"
#include "display/hagl/hagl_extensions.h"
#include "esp_log.h"

#define LOGGER_TAG "hud_controller"
#define REQUEST_PAYLOAD_BUFFER_SIZE 128
#define ERROR_BUFFER_SIZE 128

esp_err_t POST_hud_handler(httpd_req_t *req);
static esp_err_t OPTIONS_handler(httpd_req_t *req);

static const httpd_uri_t _endpoints[] = {
    {.uri = "/hud",
     .method = HTTP_POST,
     .handler = POST_hud_handler},
    {.uri = "/hud",
     .method = HTTP_OPTIONS,
     .handler = OPTIONS_handler}};

void initialize_hud_controller()
{
    info_hud_initialize();
    info_hud_draw_background("ff0000");
    info_hud_update_top("R0", 2);
    info_hud_update_bottom("E1", 2);
}

httpd_uri_t *get_hud_controller_endpoints()
{
    return _endpoints;
}

uint8_t get_hud_controller_endpoint_count()
{
    return (uint8_t)(sizeof(_endpoints) / sizeof(_endpoints[0]));
}

static esp_err_t OPTIONS_handler(httpd_req_t *request)
{
    // answer CORS requests
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Allow-Origin", "*"));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Max-Age", "600"));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS"));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Allow-Headers", "*"));
    ESP_ERROR_CHECK(httpd_resp_set_status(request, HTTPD_204));
    ESP_ERROR_CHECK(httpd_resp_send(request, "", 0));
    return ESP_OK;
}

esp_err_t POST_hud_handler(httpd_req_t *req)
{
    char payload_buffer[REQUEST_PAYLOAD_BUFFER_SIZE];
    char error_buffer[ERROR_BUFFER_SIZE];

    cJSON *json = webserver_try_get_json_request(
        req,
        payload_buffer, sizeof(payload_buffer) / sizeof(payload_buffer[0]),
        error_buffer, sizeof(error_buffer) / sizeof(error_buffer[0]));

    if (json == NULL)
    {
        httpd_resp_set_status(req, HTTPD_400);
        httpd_resp_send(req, error_buffer, HTTPD_RESP_USE_STRLEN);
        return ESP_FAIL;
    }

    ESP_LOGI(LOGGER_TAG, "Received json request: %s", cJSON_Print(json));
    // -------------------------------------------------------
    ESP_LOGI(LOGGER_TAG, "Testing for color changes...");
    if (cJSON_HasObjectItem(json, "rgbhex"))
    {
        cJSON *item = cJSON_GetObjectItem(json, "rgbhex");
        if (cJSON_IsString(item))
        {
            ESP_LOGI(LOGGER_TAG, "Updating background to %s", item->valuestring);
            info_hud_draw_background(item->valuestring);
        }
    }
    // -------------------------------------------------------
    ESP_LOGI(LOGGER_TAG, "Testing for top changes...");
    if (cJSON_HasObjectItem(json, "top"))
    {
        cJSON *item = cJSON_GetObjectItem(json, "top");

        if (!cJSON_IsNull(item))
        {
            ESP_LOGI(LOGGER_TAG, "Updating bottom section => %s (%i)", item->valuestring, strlen(item->valuestring));
            info_hud_update_top(item->valuestring, strlen(item->valuestring));
        }
    }
    // -------------------------------------------------------
    ESP_LOGI(LOGGER_TAG, "Testing for bottom changes...");
    if (cJSON_HasObjectItem(json, "bottom"))
    {
        cJSON *item = cJSON_GetObjectItem(json, "bottom");

        if (!cJSON_IsNull(item))
        {
            ESP_LOGI(LOGGER_TAG, "Updating bottom section => %s (%i)", item->valuestring, strlen(item->valuestring));
            info_hud_update_bottom(item->valuestring, strlen(item->valuestring));
        }
    }
    // -------------------------------------------------------
    cJSON_Delete(json);

    httpd_resp_set_status(req, HTTPD_200);
    return httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
}