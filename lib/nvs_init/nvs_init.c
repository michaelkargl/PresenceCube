#include "nvs_flash.h"
#include "nvs_init.h"
#include "nvs.h"
#include "esp_log.h"

static const char *_LOGGING_TAG = "nvs";

esp_err_t initialize_non_volatile_storage()
{
    ESP_LOGI(_LOGGING_TAG, "Initializing non volatile storage...");
    esp_err_t state = nvs_flash_init();
    if (state == ESP_ERR_NVS_NO_FREE_PAGES || state == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_LOGD(_LOGGING_TAG, "Erasing flash...");
        ESP_ERROR_CHECK(nvs_flash_erase());

        ESP_LOGD(_LOGGING_TAG, "Retrying flash initialization");
        state = nvs_flash_init();
    }

    ESP_ERROR_CHECK(state);
    ESP_LOGI(_LOGGING_TAG, "NVS initialized.");

    return ESP_OK;
}