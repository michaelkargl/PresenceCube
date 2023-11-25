#include "endpoint_base.h"

#define HTTPD_501 "501 METHOD_NOT_IMPLEMENTED"
#define HEADER_KEY__ACCESS_CONTROL__ALLOW_ORIGIN "Access-Control-Allow-Origin"
#define HEADER_KEY__ACCESS_CONTROL__MAX_AGE "Access-Control-Max-Age"
#define HEADER_KEY__ACCESS_CONTROL__ALLOW_METHODS "Access-Control-Allow-Methods"
#define HEADER_KEY__ACCESS_CONTROL__ALLOW_HEADERS "Access-Control-Allow-Headers"
#define HEADER_VALUE__ACCESS_CONTROL__ANY "*"


static void set_cors(httpd_req_t *request) {
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, HEADER_KEY__ACCESS_CONTROL__MAX_AGE, "1"));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, HEADER_KEY__ACCESS_CONTROL__ALLOW_METHODS, "PUT,POST,GET,OPTIONS"));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, HEADER_KEY__ACCESS_CONTROL__ALLOW_ORIGIN, HEADER_VALUE__ACCESS_CONTROL__ANY));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, HEADER_KEY__ACCESS_CONTROL__ALLOW_HEADERS, HEADER_VALUE__ACCESS_CONTROL__ANY));
}


void send_response(httpd_req_t *request, char *status_code, const char *payload)
{
    ESP_ERROR_CHECK(httpd_resp_set_status(request, status_code));

    set_cors(request);

    if (payload != NULL)
    {
        ESP_ERROR_CHECK(httpd_resp_send(request, payload, HTTPD_RESP_USE_STRLEN));
    }
}
