#include "nvs_flash.h"
#include "nvs_init.h"
#include "nvs.h"
#include "logger.iface.h"

static const char *LOGGING_TAG = "nvs";

esp_err_t initialize_non_volatile_storage()
{
    log_information(LOGGING_TAG, "Initializing non volatile storage...\n");
    esp_err_t state = nvs_flash_init();
    if (state == ESP_ERR_NVS_NO_FREE_PAGES || state == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        log_debug(LOGGING_TAG, "Erasing flash...\n");
        ESP_ERROR_CHECK(nvs_flash_erase());

        log_debug(LOGGING_TAG, "Retrying flash initialization\n");
        state = nvs_flash_init();
    }

    ESP_ERROR_CHECK(state);
    log_information(LOGGING_TAG, "NVS initialized.\n");

    return ESP_OK;
}