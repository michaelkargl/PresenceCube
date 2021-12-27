#include "led_controller.h"
#include "esp_log.h"
#include "webserver.h"

#define REQUEST_BODY_BUFFER_SIZE 50
#define REQUEST_ERROR_MESSAGE_BUFFER_SIZE 50

static const char *_LOGGER_TAG = "led_controller";

static esp_err_t POST_led_handler(httpd_req_t *req);

static const httpd_uri_t _post_hello = {
    .uri = "/led",
    .method = HTTP_POST,
    .handler = POST_led_handler};

static const struct led_controller_endpoints_t _home_controller_endpoints = {
    .get_hello = &_post_hello
};

const struct led_controller_endpoints_t *get_led_controller_endpoints()
{
    return &_home_controller_endpoints;
}

static esp_err_t POST_led_handler(httpd_req_t *request)
{
    char body_buffer[REQUEST_BODY_BUFFER_SIZE];
    char error_buffer[REQUEST_ERROR_MESSAGE_BUFFER_SIZE];
    memset(body_buffer, 0, sizeof(body_buffer));
    memset(error_buffer, 0, sizeof(error_buffer));

    esp_err_t state = read_request_body(request, body_buffer, sizeof(body_buffer), error_buffer, sizeof(error_buffer));
    if (state != ESP_OK) {
        httpd_resp_set_status(request, HTTPD_500);
        ESP_ERROR_CHECK(httpd_resp_send(request, error_buffer, HTTPD_RESP_USE_STRLEN));
        return ESP_FAIL;
    }

    ESP_ERROR_CHECK(httpd_resp_send(request, body_buffer, HTTPD_RESP_USE_STRLEN));
    return ESP_OK;
}