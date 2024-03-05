#include "hud_controller.h"
#include "webserver.h"
#include "info_hud.h"
#include "hagl_extensions.h"
#include "logger.h"

// TODO: move make this configurable via menuconfig
#define LOGGER_TAG "hud_controller"
#define REQUEST_PAYLOAD_BUFFER_SIZE 128
#define ERROR_BUFFER_SIZE 128


// TODO: static?
esp_err_t POST_hud_handler(httpd_req_t *req);
static esp_err_t OPTIONS_handler(httpd_req_t *req);

static const httpd_uri_t endpoints[] = {
    {.uri = "/hud",
     .method = HTTP_POST,
     .handler = POST_hud_handler},
    {.uri = "/hud",
     .method = HTTP_OPTIONS,
     .handler = OPTIONS_handler}};

// TODO: this breaks single responsibility
//       initializing a web controller should not be dealing with HUD drawing
//       move this outa heeeeeeere
void initialize_hud_controller()
{
    info_hud_initialize();
    info_hud_draw_background("ff0000");
    info_hud_update_top("R0", 2);
    info_hud_update_bottom("E1", 2);
}

const httpd_uri_t *get_hud_controller_endpoints()
{
    return endpoints;
}

uint8_t get_hud_controller_endpoint_count()
{
    return (uint8_t)(sizeof(endpoints) / sizeof(endpoints[0]));
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



// TODO: make this function smaller by introduing functions
esp_err_t POST_hud_handler(httpd_req_t *req)
{
    char payload_buffer[REQUEST_PAYLOAD_BUFFER_SIZE];
    char error_buffer[ERROR_BUFFER_SIZE];

    // TODO: move parsing of incoming json-string to request dto into separate module or function
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


    log_information(LOGGER_TAG, "Received json request: %s\n", cJSON_Print(json));
    
    // TODO: move the mutation of the hud into a separate command handler
    //       this will be especially needed once MQTT connection is implemented
    // -------------------------------------------------------
    log_information(LOGGER_TAG, "Testing for color changes...\n");
    if (cJSON_HasObjectItem(json, "rgbhex"))
    {
        cJSON *item = cJSON_GetObjectItem(json, "rgbhex");
        if (cJSON_IsString(item))
        {
            log_information(LOGGER_TAG, "Updating background to %s\n", item->valuestring);
            info_hud_draw_background(item->valuestring);
        }
    }
    
    // TODO: move the mutation of the hud into a separate command handler
    //       this will be especially needed once MQTT connection is implemented
    // -------------------------------------------------------
    log_information(LOGGER_TAG, "Testing for top changes...\n");
    if (cJSON_HasObjectItem(json, "top"))
    {
        cJSON *item = cJSON_GetObjectItem(json, "top");

        if (!cJSON_IsNull(item))
        {
            log_information(LOGGER_TAG, "Updating bottom section => %s (%i)\n", item->valuestring, strlen(item->valuestring));
            info_hud_update_top(item->valuestring, strlen(item->valuestring));
        }
    }
    
    // TODO: move the mutation of the hud into a separate command handler
    //       this will be especially needed once MQTT connection is implemented
    // -------------------------------------------------------
    log_information(LOGGER_TAG, "Testing for bottom changes...\n");
    if (cJSON_HasObjectItem(json, "bottom"))
    {
        cJSON *item = cJSON_GetObjectItem(json, "bottom");

        if (!cJSON_IsNull(item))
        {
            log_information(LOGGER_TAG, "Updating bottom section => %s (%i)\n", item->valuestring, strlen(item->valuestring));
            info_hud_update_bottom(item->valuestring, strlen(item->valuestring));
        }
    }
    // -------------------------------------------------------
    cJSON_Delete(json);

    httpd_resp_set_status(req, HTTPD_200);
    return httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
}