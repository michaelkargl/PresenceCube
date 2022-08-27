#pragma once
#include "cJSON.h"
#include "set_led_request_dto_t.h"

struct set_led_request_dto_t json_parse__set_led_request_dto(cJSON *json);