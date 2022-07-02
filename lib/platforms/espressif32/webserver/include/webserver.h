#pragma once
#include "esp_err.h"
#include "esp_http_server.h"
#include "cJSON.h"


httpd_handle_t webserver_start();
esp_err_t webserver_stop();

esp_err_t webserver_register_endpoint(const httpd_uri_t *endpoint);
esp_err_t webserver_register_endpoints(const httpd_uri_t* endpoints, const uint32_t endpoint_count);

esp_err_t webserver_read_request_body(
    httpd_req_t *request,
    char *body_buffer, uint8_t body_buffer_size,
    char *error_buffer, uint8_t error_buffer_size);

esp_err_t webserver_try_get_text_request(
    httpd_req_t *request,
    char *payload_buffer, uint8_t payload_buffer_size,
    char *error_buffer, uint8_t error_buffer_size);

cJSON *webserver_try_get_json_request(
    httpd_req_t *request,
    char *payload_buffer, uint8_t payload_buffer_size,
    char *error_buffer, uint8_t error_buffer_size);

cJSON *webserver_try_parse_json(
    char *json_string, 
    char *error_buffer, uint8_t error_buffer_size);

esp_err_t webserver_test_request_size_within_bounds(
    httpd_req_t *request, uint8_t request_body_buffer_size,
    char *error_buffer, uint8_t error_buffer_size);