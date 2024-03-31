#pragma once
#include "get_led_query.h"
#include "get_led_query_response.h"

/**
 * @brief Handles the input request and provides a response.
 * 
 * @param request A structure representing the desire to query some information.
 *                It provides the handler with all the necessary query parameters.
 * @return get_led_query_response_t
 * @see https://github.com/jbogard/MediatR/wiki#requestresponse
 */
get_led_query_response_t get_led_query_handler__handle(const get_led_query_t request);
