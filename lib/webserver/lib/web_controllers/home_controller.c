#include "home_controller.h"

static esp_err_t GET_hello_handler(httpd_req_t *req);

static const httpd_uri_t _get_hello = {
    .uri = "/hello",
    .method = HTTP_GET,
    .handler = GET_hello_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx = "Hello World!"};

static const struct home_controller_endpoints_t _home_controller_endpoints = {
    .get_hello = &_get_hello
};

const struct home_controller_endpoints_t *get_home_controller_endpoints()
{
    return &_home_controller_endpoints;
}

static esp_err_t GET_hello_handler(httpd_req_t *request)
{
    /* Send a simple response */
    const char response[] = "Sup!";
    httpd_resp_send(request, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}