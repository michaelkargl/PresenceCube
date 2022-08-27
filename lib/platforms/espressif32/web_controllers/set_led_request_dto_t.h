#pragma once
#include "stdint.h"

struct set_led_request_dto_t
{
    uint8_t id;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
