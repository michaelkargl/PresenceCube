#include "nvs_flash.h"
#include "nvs_init.h"
#include "nvs.h"
#include "logger.h"

static const char *_LOGGING_TAG = "nvs";

esp_err_t initialize_non_volatile_storage()
{
    log_information(_LOGGING_TAG, "Initializing non volatile storage...");
    esp_err_t state = nvs_flash_init();
    if (state == ESP_ERR_NVS_NO_FREE_PAGES || state == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        log_debug(_LOGGING_TAG, "Erasing flash...");
        ESP_ERROR_CHECK(nvs_flash_erase());

        log_debug(_LOGGING_TAG, "Retrying flash initialization");
        state = nvs_flash_init();
    }

    ESP_ERROR_CHECK(state);
    log_information(_LOGGING_TAG, "NVS initialized.");

    return ESP_OK;
}