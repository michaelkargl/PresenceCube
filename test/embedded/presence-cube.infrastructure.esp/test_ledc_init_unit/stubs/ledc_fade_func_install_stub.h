#pragma once
#include "esp_err.h"


esp_err_t ledc_fade_func_install_stub(int intr_alloc_flags);
void ledc_fade_func_install_stub__reset();
uint8_t ledc_fade_func_install_stub__get_call_count();