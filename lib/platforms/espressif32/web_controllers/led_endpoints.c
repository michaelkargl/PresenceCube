#include "led_endpoints.h"
#include "led_controller.h"
#include "delay_service.h"
#include "stdbool.h"
#include "using_json.h"
#include "using_free.h"
#include "ledc_rgb_led_t.h"
#include "json_serialize__get_led_query_response_t.h"
#include "json_parse__set_led_request_dto_t.h"
#include "esp_err.h"

// focuses on HTTP specifics and translations between http and dto

// TODO: move configuration into a separate configuration header
#define REQUEST_BODY_BUFFER_SIZE 50
#define REQUEST_ERROR_MESSAGE_BUFFER_SIZE 50
#define LED_FADE_MILLISECONDS 4000


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

const httpd_uri_t *get_led_controller_endpoints()
{
    return _home_controller_endpoints;
}

uint8_t get_led_controller_endpoint_count()
{
    return (uint8_t)(sizeof(_home_controller_endpoints) / sizeof(_home_controller_endpoints[0]));
}


static esp_err_t OPTIONS_handler(httpd_req_t *request)
{
    send_response(request, HTTPD_204, "Cors approved ;)");
    return ESP_OK;
}



// TODO: integration test
static esp_err_t GET_leds_handler(httpd_req_t *request)
{
    volatile esp_err_t status_code = ESP_OK;
    volatile CEXCEPTION_T ex;
    Try {
        get_led_query_response_t response = led_controller__GET_leds_handler();

        const char *response_string = json_stringify__get_led_query_response_t(&response);
        THROW_ARGUMENT_NULL_IF_NULL(response_string);
        
        USING(response_string, {
            send_response(request, HTTPD_200, response_string);
        });
    } Catch(ex) {
        
        // TODO: error responses could be handled in a separate module.
        //       send_error_response(CException_T ex) that triggers
        //       some sort of error handling middleware

        switch(ex) {
            case ERROR_CODE_RESOURCE_NOT_FOUND: {
                send_response(request, HTTPD_404, "No registered LEDs could be found.");
                status_code = ESP_ERR_NOT_FOUND;
                break;
            } default: {
                send_response(request, HTTPD_500, NULL);
                status_code = ESP_FAIL;
                break;
            }
        }
    }

    heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
    return status_code;
}

static esp_err_t POST_led_handler(httpd_req_t *request)
{
    char body_buffer[REQUEST_BODY_BUFFER_SIZE];
    char error_buffer[REQUEST_ERROR_MESSAGE_BUFFER_SIZE];
    volatile esp_err_t status_code = ESP_OK;

    cJSON *json = webserver_try_get_json_request(
        request,
        body_buffer, sizeof(body_buffer) / sizeof(body_buffer[0]),
        error_buffer, sizeof(error_buffer) / sizeof(error_buffer[0]));

    volatile CEXCEPTION_T ex;
    Try
    {
        USING_JSON(json, {
            struct set_led_request_dto_t dto = json_parse__set_led_request_dto(json);
            led_controller__POST_led_handler(&dto);
        });

        send_response(request, HTTPD_200, "OK");
    }
    Catch(ex)
    {
        switch(ex) {
            case ERROR_CODE_CALL_NOT_IMPLEMENTED: {
                send_response(request, HTTPD_501, "Not implemented");
                status_code = ESP_ERR_NOT_SUPPORTED;
                break;
            } default: {
                send_response(request, HTTPD_500, "An error occured.");
                status_code = ESP_FAIL;
            }
        }
    }

    heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
    return status_code;
}