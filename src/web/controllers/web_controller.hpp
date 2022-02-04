#pragma once
#include "esp_http_server.h"
#include "esp_err.h"
#include "esp_log.h"
//TODO: Refactor LED controller and remove webserver.h
#include "webserver.h"

#define LOGGER_TAG "WebController"

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

        /**
         * @brief Provides an application specific way to send http reqponses.
         * 
         * @param request The request to respond to
         * @param status_code The HTTP status code to send
         * @param payload An optional payload to send
         */
        static void send_response(httpd_req_t *request, const char* status_code, const char* payload);

        /**
         * @brief Sends the data to the client in chunks of specified size.
         * 
         * @param request The request to respond to
         * @param status_code The HTTP status code to send
         * @param data The data to send in chunks
         * @param chunk_count How many parts the data is chunked into
         */
        void send_chunked_response(
            httpd_req_t *request, 
            const char* status_code, 
            const unsigned char* data,
            const uint32_t data_length,
            const uint8_t chunk_count
        );

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

void WebController::send_response(httpd_req_t *request, const char* status_code, const char* payload) {
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Allow-Origin", "*"));
    ESP_ERROR_CHECK(httpd_resp_set_status(request, status_code));
    ESP_ERROR_CHECK(httpd_resp_send(request, payload, HTTPD_RESP_USE_STRLEN));
}

void WebController::send_chunked_response(
    httpd_req_t *request, 
    const char* status_code, 
    const unsigned char* data,
    const uint32_t data_length,
    const uint8_t chunk_count
) {
    auto chunk_size = data_length / chunk_count;
    auto unsent_bytes = data_length;
    char buffer[chunk_size];
    
    ESP_LOGD(LOGGER_TAG, "Data begin: %p", data);
    ESP_LOGD(LOGGER_TAG, "Data end: %p", &data[data_length-1]);

    for (auto i = 0; unsent_bytes > 0; i++) {
        ESP_LOGD(LOGGER_TAG, "Entering loop %i with %i unsent elements", i, unsent_bytes);
        auto data_fits_chunk = unsent_bytes >= chunk_size;
        auto copy_span = data_fits_chunk ? chunk_size : unsent_bytes; 
        
        auto chunk_start = data_length - unsent_bytes;
        auto chunk_end = chunk_start + copy_span;
        auto copy_start_position = &data[chunk_start];
        auto copy_end_position = &data[chunk_end];
        
        ESP_LOGD(LOGGER_TAG, "Copying %i elements startign from position %i(%p) to position %i(%p)", 
            data_length, 
            chunk_start, copy_start_position,
            chunk_end, copy_end_position);
        memcpy(buffer, copy_start_position, copy_span);
        
        // flush
        httpd_resp_send_chunk(request, buffer, copy_span);
        unsent_bytes -= copy_span;
    }

    // marks the end of chunked response
    httpd_resp_send_chunk(request, buffer, 0);
}