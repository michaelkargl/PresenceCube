#pragma once
#include "web_controller.hpp"
#include "esp_http_server.h"
#include "display/heads_up_display.hpp"
#include "esp_err.h"
#include "string.h"


#define REQUEST_PAYLOAD_BUFFER_SIZE 128
#define ERROR_BUFFER_SIZE 128

esp_err_t POST_hud_handler(httpd_req_t *req);

class HudController : public WebController {
    public:
        HudController(IHeadsUpDisplay* display) : _display(display) {
        }
        esp_err_t POST_hud_handler(httpd_req_t *req);
        IHeadsUpDisplay* _display;

    private:
        constexpr static const char* _logger_tag = "HudController";
        const httpd_uri_t _endpoints[1] = {
            { "/hud", HTTP_POST, &::POST_hud_handler, this}
        };

    const httpd_uri_t* getEndpoints(uint32_t* out_endpoint_count) {
        *out_endpoint_count = sizeof(_endpoints) / sizeof(_endpoints[0]);
        return _endpoints;
    }
};

esp_err_t POST_hud_handler(httpd_req_t *req) {
    HudController* controller = (HudController*) req->user_ctx;
    return controller->POST_hud_handler(req);
}

esp_err_t HudController::POST_hud_handler(httpd_req_t *req) {
    char payload_buffer[REQUEST_PAYLOAD_BUFFER_SIZE];
    char error_buffer[ERROR_BUFFER_SIZE];

    cJSON* json = this->try_get_json_request(
        req, 
        payload_buffer, sizeof(payload_buffer)/sizeof(payload_buffer[0]),
        error_buffer, sizeof(error_buffer)/sizeof(error_buffer[0])
    );

    if ( json == NULL ) {
        httpd_resp_set_status(req, HTTPD_400);
        httpd_resp_send(req, error_buffer, HTTPD_RESP_USE_STRLEN);
        return ESP_FAIL;
    }

    ESP_LOGD(this->_logger_tag, "Received json request: %s", cJSON_Print(json));

    ESP_LOGD(this->_logger_tag, "Testing for center value...");
    if ( cJSON_HasObjectItem(json, "centerValue")) {
        cJSON* item = cJSON_GetObjectItem(json, "centerValue");

        if (!cJSON_IsNull(item))
            this->_display->updateCenterValue(item->valuestring, strlen(item->valuestring));
    }

    ESP_LOGD(this->_logger_tag, "Testing for center label...");
    if ( cJSON_HasObjectItem(json, "centerLabel") ) {
        cJSON* item = cJSON_GetObjectItem(json, "centerLabel");

        if (!cJSON_IsNull(item))
            this->_display->updateCenterLabel(item->valuestring, strlen(item->valuestring));
    }

    ESP_LOGD(this->_logger_tag, "Testing for top section...");
    if ( cJSON_HasObjectItem(json, "top") ) {
        cJSON* item = cJSON_GetObjectItem(json, "top");
        
        if (!cJSON_IsNull(item))
            this->_display->updateTopText(item->valuestring, strlen(item->valuestring));
    }

    ESP_LOGD(this->_logger_tag, "Testing for bottom section...");
    if ( cJSON_HasObjectItem(json, "bottom") ) {
        cJSON* item = cJSON_GetObjectItem(json, "bottom");
        
        if (!cJSON_IsNull(item))
            this->_display->updateBottomText(item->valuestring, strlen(item->valuestring));
    }
    
    ESP_LOGD(this->_logger_tag, "Flushing updates...");
    this->_display->flushUpdates();

    cJSON_free(json);

    httpd_resp_set_status(req, HTTPD_200);
    return httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
}