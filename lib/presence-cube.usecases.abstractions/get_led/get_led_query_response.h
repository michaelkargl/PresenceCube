#pragma once
#include "rgb_led_diode.h"

typedef struct {
    // A bag filled with LEDs.
    const rgb_led_diode_bag_t* led_bag;
} get_led_query_response_t;
