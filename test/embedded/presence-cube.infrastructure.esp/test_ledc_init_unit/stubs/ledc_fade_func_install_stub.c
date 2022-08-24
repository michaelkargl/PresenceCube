#include "ledc_fade_func_install_stub.h"

static uint8_t _call_count = 0;

esp_err_t ledc_fade_func_install_stub(int intr_alloc_flags) {
    _call_count++;
    return ESP_OK;
}

void ledc_fade_func_install_stub__reset() {
    _call_count = 0;
}

uint8_t ledc_fade_func_install_stub__get_call_count() {
    return _call_count;
}

