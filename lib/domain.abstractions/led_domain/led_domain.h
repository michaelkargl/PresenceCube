#pragma once
#include "stdint.h"
#include "stdbool.h"

typedef struct {
    /**
     * @brief Set this flag to true when initializing the struct. This allows both
     *        application and tests to detect when the struct is uninitialized
     *        I.e.: if variable is `(led_domain_t){}`
     */
    bool initialized;
    /**
     * @brief Uniquely identifies the led system wide.
     */
    uint8_t id;
    uint8_t brightness_percent;
    const char display_name[5];
} led_domain_t;
