#include "spiffs.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "logger.h"

static const char *TAG = "spiffs";

static void _spiffs_display(const char * path) {
	DIR* dir = opendir(path);
	assert(dir != NULL);
	while (true) {
		struct dirent*pe = readdir(dir);
		if (!pe) break;
		log_information(__FUNCTION__,"d_name=%s d_ino=%d d_type=%x\n", pe->d_name,pe->d_ino, pe->d_type);
	}
	closedir(dir);
}

void initialize_spiffs(esp_vfs_spiffs_conf_t* configuration) {
    log_information(TAG, "Initializing SPIFFS\n");

	// Use settings defined above toinitialize and mount SPIFFS filesystem.
	// Note: esp_vfs_spiffs_register is anall-in-one convenience function.
	esp_err_t ret = esp_vfs_spiffs_register(configuration);

	if (ret != ESP_OK) {
		if (ret == ESP_FAIL) {
			log_error(TAG, "Failed to mount or format filesystem\n");
		} else if (ret == ESP_ERR_NOT_FOUND) {
			log_error(TAG, "Failed to find SPIFFS partition\n");
		} else {
			log_error(TAG, "Failed to initialize SPIFFS (%s)\n",esp_err_to_name(ret));
		}
		return;
	}

	size_t total = 0, used = 0;
	ret = esp_spiffs_info(NULL, &total,&used);
	
	if (ret != ESP_OK) {
		log_error(TAG,"Failed to get SPIFFS partition information (%s)\n",esp_err_to_name(ret));
	} else {
		log_information(TAG,"Partition size: total: %d, used: %d\n", total, used);
	}

	_spiffs_display(configuration->base_path);
}