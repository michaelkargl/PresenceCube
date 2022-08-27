#pragma once
#include "get_led_query_response.h"
#include "set_led_request_dto_t.h"

get_led_query_response_t led_controller__GET_leds_handler();
void led_controller__POST_led_handler(struct set_led_request_dto_t *set_led_request);
