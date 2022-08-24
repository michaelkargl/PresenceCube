#include "ledc_channel_config_stub.h"

static uint8_t _call_count = 0;

esp_err_t ledc_channel_config_stub(const ledc_channel_config_t* ledc_conf) {
    _call_count++;
    return ESP_OK;
}

uint8_t ledc_channel_config_stub__get_call_count() {
    return _call_count;
}

void ledc_channel_config_stub__reset() {
    _call_count = 0;
}