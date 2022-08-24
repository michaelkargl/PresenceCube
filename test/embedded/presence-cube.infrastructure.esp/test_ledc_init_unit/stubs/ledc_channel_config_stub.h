#pragma once
#include "esp_err.h"
#include "hal/ledc_types.h"


esp_err_t ledc_channel_config_stub(const ledc_channel_config_t* ledc_conf);
void ledc_channel_config_stub__reset();
uint8_t ledc_channel_config_stub__get_call_count();