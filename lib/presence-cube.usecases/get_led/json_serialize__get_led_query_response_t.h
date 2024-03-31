#pragma once
#include "get_led/get_led_query_response.h"

/**
 * @brief Json stringifies the given structure
 *
 * @param response the structure to stringify
 * @return const char* an unmanaged json string that needs to be disposed of manually after use.
 */
const char *json_stringify__get_led_query_response_t(get_led_query_response_t *response);