#pragma once
#include "civetweb.h"
#include "stdbool.h"

/**
 * @brief Registers endpoints to the live web server
 * @param cancellation_token a token that, if set to true, will signal cancellation to its owner
 */
void controller_registrar__register(
    struct mg_context *webserver_context,
    bool *cancellation_token);