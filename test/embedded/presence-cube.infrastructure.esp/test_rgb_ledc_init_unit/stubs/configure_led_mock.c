#include "configure_led_mock.h"

static uint8_t call_counter = 0;


void configure_led_mock__reset() {
    call_counter = 0;
}

uint8_t configure_led_mock__get_callcount() {
    return call_counter;
}

void configure_led_mock(const struct ledc_led_t* led) {
    call_counter++;
}