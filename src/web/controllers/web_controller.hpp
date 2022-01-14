#pragma once
#include "esp_http_server.h"
#include "esp_err.h"
#include "esp_log.h"
//TODO: Refactor LED controller and remove webserver.h
#include "webserver.h"


class WebController {
    public:
        virtual const httpd_uri_t* getEndpoints(uint32_t* out_endpoint_count) = 0;
    
    protected:
        /**
         * @brief Tries to retrieve the request content as raw text.
         * 
         * @param request The request to parse the body from
         * @param payload_buffer Char array to read the request body into. Access is trimmed.
         * @param payload_buffer_size The size of the char array
         * @param error_buffer Char array to read eventual error messages into. Access is trimmed.
         * @param error_buffer_size The size of the char array
         * @return esp_err_t 
         */
        esp_err_t try_get_text_request(
            httpd_req_t *request,
            char *payload_buffer, uint8_t payload_buffer_size,
            char *error_buffer, uint8_t error_buffer_size);

        /**
         * @brief Tries to interpret the request payload as a json object
         * 
         * @param request The request to parse the body from
         * @param payload_buffer Char array to read the request body into. Access is trimmed.
         * @param payload_buffer_size The size of the char array
         * @param error_buffer Char array to read eventual error messages into. Access is trimmed.
         * @param error_buffer_size The size of the char array
         * @return cJSON* json object. Don't forget to cJSON_free!
         */
        cJSON *try_get_json_request(
            httpd_req_t *request,
            char *payload_buffer, uint8_t payload_buffer_size,
            char *error_buffer, uint8_t error_buffer_size);

    private:
        /**
         * @brief Retrieves the request body as string
         * 
         * @param request The request to parse the body from
         * @param body_buffer Char array to read the request body into. Access is trimmed.
         * @param body_buffer_size The size of the char array
         * @param error_buffer Char array to read eventual error messages into. Access is trimmed.
         * @param error_buffer_size The size of the char array
         * @return esp_err_t 
         */
        esp_err_t read_request_body(
            httpd_req_t *request,
            char *body_buffer, uint8_t body_buffer_size,
            char *error_buffer, uint8_t error_buffer_size
        );

        /**
         * @brief Tries to parse json
         * @return a pointer to the cjson object or NULL if unsuccessful. The error message buffer might contain
         *         clues on what went wrong. Be sure to free up returned pointer with cJSON functions.
         */
        cJSON *try_parse_json(char *json_string, char *error_buffer, uint8_t error_buffer_size);

        /**
         * @brief Tests if the received request can fit within the defined buffer
         * 
         * @param request Request to parse the body from
         * @param request_body_buffer_size The size of the body buffer char array
         * @param error_buffer A char array to read eventual error messages into. Access is trimmed.
         * @param error_buffer_size The size of the error buffer char array
         * @return esp_err_t 
         */
        esp_err_t test_request_size_within_bounds(
            httpd_req_t *request, uint8_t request_body_buffer_size,
            char *error_buffer, uint8_t error_buffer_size
        );
};


/**
 * @brief Tries to parse json
 * @return a pointer to the cjson object or NULL if unsuccessful. The error message buffer might contain
 *         clues on what went wrong. Be sure to free up returned pointer with cJSON functions.
 */
cJSON* WebController::try_parse_json(char *json_string, char *error_buffer, uint8_t error_buffer_size)
{
    return ::try_parse_json(json_string, error_buffer, error_buffer_size);
}


esp_err_t WebController::read_request_body(
    httpd_req_t *request,
    char *body_buffer, uint8_t body_buffer_size,
    char *error_buffer, uint8_t error_buffer_size)
{
    return ::read_request_body(
        request, 
        body_buffer, body_buffer_size, 
        error_buffer, error_buffer_size
    );
}


esp_err_t WebController::test_request_size_within_bounds(
    httpd_req_t *request, uint8_t request_body_buffer_size,
    char *error_buffer, uint8_t error_buffer_size)
{
    return ::test_request_size_within_bounds(
        request,
        request_body_buffer_size,
        error_buffer, error_buffer_size
    );
}


esp_err_t WebController::try_get_text_request(
    httpd_req_t *request,
    char *payload_buffer, uint8_t payload_buffer_size,
    char *error_buffer, uint8_t error_buffer_size)
{
    return ::try_get_text_request(
        request,
        payload_buffer, payload_buffer_size,
        error_buffer, error_buffer_size
    );
}


cJSON* WebController::try_get_json_request(
    httpd_req_t *request,
    char *payload_buffer, uint8_t payload_buffer_size,
    char *error_buffer, uint8_t error_buffer_size)
{
    return ::try_get_json_request(
        request, 
        payload_buffer, payload_buffer_size,
        error_buffer, error_buffer_size
    );
}