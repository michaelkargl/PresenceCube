#include "esp_log.h"
#include "display_init.h"
#include <gdew027w3T.h>
#include "display/epaper_display.hpp"

static const char* TAG = "display_init";
static FT6X36* _touch_panel;
static EpdSpi* _spi_io_interface;
static Gdew027w3T* _gdew27w3tDisplay;
static EpaperDisplay* _display;

IDisplay* initialize_display() {
    const bool debug_mode = false;
    
    // allocate memory
    _touch_panel = (FT6X36*) malloc(sizeof(FT6X36));
    _spi_io_interface = (EpdSpi*) malloc(sizeof(EpdSpi));
    _gdew27w3tDisplay = (Gdew027w3T*) malloc(sizeof(Gdew027w3T));
    _display = (EpaperDisplay*) malloc(sizeof(EpaperDisplay));

    // fill empty memory with instances
    new(_touch_panel) FT6X36(CONFIG_TOUCH_INT);
    new(_spi_io_interface) EpdSpi();
    new(_gdew27w3tDisplay) Gdew027w3T(*_spi_io_interface, *_touch_panel);
    new(_display) EpaperDisplay(_gdew27w3tDisplay);
    
    ESP_LOGD(TAG, "Initializing display with debug mode %s", debug_mode ? "enabled" : "disabled");
    _gdew27w3tDisplay->init(debug_mode);

    ESP_LOGD(TAG, "Clearing screen...");
    _display->fillScreen(EPD_WHITE);
    _display->flushUpdates();

    return _display;
}

void deinitialize_display() {
    free(_display);          _display = NULL;
    free(_gdew27w3tDisplay); _gdew27w3tDisplay = NULL;
    free(_spi_io_interface); _spi_io_interface = NULL;
    free(_touch_panel);      _touch_panel = NULL;
}
