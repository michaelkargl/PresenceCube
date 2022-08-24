#pragma once
// TODO: remove the domain from the name (led_diode_t?)

#include "stdint.h"
#include "stdbool.h"

typedef struct {
    // TODO: standardize is_initialized flag using a macro
    bool is_initialized;
    uint8_t id;
    uint8_t brightness_percent;
    char display_name[5];
} led_domain_t;



