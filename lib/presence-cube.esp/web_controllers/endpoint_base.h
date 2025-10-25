#pragma once
#include "webserver.h"

#define HTTPD_501 "501 METHOD_NOT_IMPLEMENTED"

// TODO: rename to include module name
void send_response(httpd_req_t *request, char *status_code, const char *payload);