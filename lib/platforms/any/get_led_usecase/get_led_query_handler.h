#pragma once
#include "get_led_query.h"
#include "get_led_query_response.h"


bool get_led_query_handler_initialized();
void get_led_query_handler_init();
void get_led_query_handler_deinit();

/**
 * @brief Handles the input request and provides a response.
 * 
 * @param request A structure representing the desire to query some information.
 *                It provides the handler with all the necessary query parameters.
 * @return const get_led_query_response_t
 * @see https://github.com/jbogard/MediatR/wiki#requestresponse
 */
const get_led_query_response_t handle_get_led_query(const get_led_query_t request);