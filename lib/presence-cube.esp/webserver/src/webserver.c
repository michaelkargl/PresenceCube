#include "webserver.h"
#include "logger.iface.h"
#include "esp_err.h"

static const char *LOGGER_TAG = "webserver";
static httpd_handle_t server = NULL;

/**
 * @brief Tries to parse json
 * @return a pointer to the cjson object or NULL if unsuccessful. The error message buffer might contain
 *         clues on what went wrong. Be sure to free up returned pointer with cJSON functions.
 */
cJSON *webserver_try_parse_json(char *json_string, char *error_buffer, uint8_t error_buffer_size)
{
    log_debug(LOGGER_TAG, "Trying to parse json message: %s\n", json_string);
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            memset(error_buffer, 0, error_buffer_size);
            snprintf(
                error_buffer,
                error_buffer_size,
                "An error occured while parsing request JSON body: %s",
                error_ptr);

            log_error(LOGGER_TAG, "%s\n", error_buffer);
            log_error(LOGGER_TAG, "Erroneous json: %s\n", error_ptr);
        }

        log_debug(LOGGER_TAG, "Message parsing failed with unknown reason\n");
        return NULL;
    }

    log_debug(LOGGER_TAG, "Json successfully parsed.\n");
    return json;
}

esp_err_t webserver_read_request_body(
    httpd_req_t *request,
    char *body_buffer, uint8_t body_buffer_size,
    char *error_buffer, uint8_t error_buffer_size)
{
    log_debug(LOGGER_TAG, "Trying to read requet body...\n");
    memset(error_buffer, 0, error_buffer_size);
    memset(body_buffer, 0, body_buffer_size);

    int body_read_successfully = httpd_req_recv(request, body_buffer, body_buffer_size);
    if (body_read_successfully <= 0)
    {
        memset(error_buffer, 0, error_buffer_size);
        snprintf(
            error_buffer,
            error_buffer_size,
            "Retrieving the message body failed with status %i.", body_read_successfully);

        log_error(LOGGER_TAG, "%s\n", error_buffer);
        log_error(LOGGER_TAG, "This might be due to Field not found / Invalid request / Null arguments.\n");
        return ESP_FAIL;
    }

    log_debug(LOGGER_TAG, "Request body successfully read: %s\n", body_buffer);
    return ESP_OK;
}

esp_err_t webserver_test_request_size_within_bounds(
    httpd_req_t *request, uint8_t request_body_buffer_size,
    char *error_buffer, uint8_t error_buffer_size)
{
    log_debug(LOGGER_TAG, "Testing if request body fits buffer size.\n");
    if (request->content_len > request_body_buffer_size)
    {
        snprintf(
            error_buffer,
            error_buffer_size,
            "Request body size of %i exceeds buffer limit of %i.",
            request->content_len, request_body_buffer_size);

        log_error(LOGGER_TAG, "%s\n", error_buffer);
        return ESP_FAIL;
    }

    log_debug(LOGGER_TAG, "Request body fits.\n");
    return ESP_OK;
}

/**
 * @brief Allocates necessary space and starts webserver
 * @return httpd_handle_t webserver reference
 */
httpd_handle_t webserver_start()
{
    if (server != NULL)
    {
        log_warning(LOGGER_TAG, "Trying to reinitialize running webserver %p. Ignoring.\n", server);
        return ESP_OK;
    }

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    log_information(LOGGER_TAG, "Configuring web server...\n");
    log_information(LOGGER_TAG, "Maximum allowed numbers of endpoints: %i\n", config.max_uri_handlers);

    const bool purge_least_recently_used_connection = true;
    config.lru_purge_enable = purge_least_recently_used_connection;

    log_information(LOGGER_TAG, "Starting server on port: '%d'\n", config.server_port);

    ESP_ERROR_CHECK(httpd_start(&server, &config));

    log_information(LOGGER_TAG, "Server started\n");
    return server;
}

esp_err_t webserver_stop()
{
    log_information(LOGGER_TAG, "Stopping and deallocating http server: %p\n", server);
    return httpd_stop(server);
}

esp_err_t webserver_register_endpoint(const httpd_uri_t *endpoint)
{
    const char* logger_tag = "webserver_register_endpoint";
    log_information(logger_tag, "Registering uri handler %s method %i\n", endpoint->uri, endpoint->method);
    log_debug(logger_tag, "Uri handler pointer: %p\n", endpoint->handler);

    esp_err_t status = httpd_register_uri_handler(server, endpoint);
    log_information(logger_tag, "Registration status of %s: %i\n", endpoint->uri, status);

    return status;
}

esp_err_t webserver_register_endpoints(const httpd_uri_t* endpoints, const uint32_t endpoint_count) {
    log_information("webserver_register_endpoints", "Registering %i endpoints...\n", endpoint_count);
    for(int i = 0; i < endpoint_count; i++) {
        webserver_register_endpoint(&endpoints[i]);
    }
    return ESP_OK;
}

/**
 * @brief  Tries to retrieve read request payload into the provided payload buffer.
 * 
 * @param[in] request Webrequest
 * @param[out] payload_buffer This buffer will contain the retrieved request payload.
 * @param[in] payload_buffer_size String length
 * @param[out] error_buffer This buffer will be filled with explanations of certain failures.
 * @param[in] error_buffer_size String length
 * @return ESP_OK, ESP_FAIL
 */
esp_err_t webserver_try_get_text_request(
    httpd_req_t *request,
    char *payload_buffer, uint8_t payload_buffer_size,
    char *error_buffer, uint8_t error_buffer_size)
{
    memset(payload_buffer, 0, payload_buffer_size);
    memset(error_buffer, 0, error_buffer_size);

    bool request_fits_buffer = webserver_test_request_size_within_bounds(request, payload_buffer_size, error_buffer, error_buffer_size) == ESP_OK;
    if (!request_fits_buffer)
    {
        return ESP_FAIL;
    }

    bool body_retrieved = webserver_read_request_body(request, payload_buffer, payload_buffer_size, error_buffer, error_buffer_size) == ESP_OK;
    if (!body_retrieved)
    {
        return ESP_FAIL;
    }

    return ESP_OK;
}

/**
 * @brief Tries to parse the requests json payload.
 * @param[out] payload_buffer This buffer will contain the retrieved request payload.
 * @param[in] payload_buffer_size String length
 * @param[out] error_buffer This buffer will be filled with explanations of certain failures.
 * @param[in] error_buffer_size String length
 * @return Disposable JSON object or NULL (dispose using cJSON)
 * @remark Disposable
 */
cJSON *webserver_try_get_json_request(
    httpd_req_t *request,
    char *payload_buffer, uint8_t payload_buffer_size,
    char *error_buffer, uint8_t error_buffer_size)
{
    memset(payload_buffer, 0, payload_buffer_size);
    memset(error_buffer, 0, error_buffer_size);

    bool payload_retrieved = webserver_try_get_text_request(
                                 request,
                                 payload_buffer, payload_buffer_size,
                                 error_buffer, error_buffer_size) == ESP_OK;
    if (!payload_retrieved)
        return NULL;

    cJSON *json = webserver_try_parse_json(payload_buffer, error_buffer, error_buffer_size);
    if (json == NULL)
        return NULL;

    return json;
}