#pragma once
#include "esp_err.h"
#include "hal/ledc_types.h"

esp_err_t ledc_timer_config_stub(const ledc_timer_config_t* timer_conf);
void ledc_timer_config_stub__reset();
uint8_t ledc_timer_config_stub__get_call_count();