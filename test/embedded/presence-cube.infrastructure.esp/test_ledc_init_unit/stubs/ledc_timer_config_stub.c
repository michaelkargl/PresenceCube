#include "ledc_timer_config_stub.h"

static uint8_t call_count = 0;

esp_err_t ledc_timer_config_stub(const ledc_timer_config_t* timer_conf) {
    call_count++;
    return ESP_OK;
}

void ledc_timer_config_stub__reset() {
    call_count = 0;
}

uint8_t ledc_timer_config_stub__get_call_count() {
    return call_count;
}