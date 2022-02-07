#define SCREEN_TYPE 0
#if SCREEN_TYPE == 0
    #define HAGL_SCREEN
#elif SCREEN_TYPE == 1
    #define EPAPER_SCREEN
#endif

#include "display_init.h"
#include "esp_log.h"
#include "display.hpp"

#if defined(EPAPER_SCREEN)
    #include "display/epaper/epaper_display.hpp"
    #include <gdew027w3T.h>
    #define SCREEN_WIDTH 320
    #define SCREEN_HEIGHT 240
#elif defined(HAGL_SCREEN)
    #include "display/hagl/hagl_display.hpp"
    #define SCREEN_WIDTH CONFIG_MIPI_DISPLAY_WIDTH
    #define SCREEN_HEIGHT CONFIG_MIPI_DISPLAY_HEIGHT
#endif

static IDisplay *_display;

static const char* TAG = "display_init";

#if defined(EPAPER_SCREEN)
    static void _initialize_epaper_display() {
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
    }
#elif defined(HAGL_SCREEN)
    static void _initialize_hagl_display() {
        Size2D screen_size = {SCREEN_WIDTH, SCREEN_HEIGHT};

        // TODO: When freeing/deleting _display, is ~HaglDisplay still called?
        _display = new HaglDisplay(screen_size);
    }
#endif

IDisplay* initialize_display() {
    ESP_LOGD(TAG, "Initializing display...");

    #if defined(EPAPER_SCREEN)
        _initialize_epaper_display();
    #elif defined(HAGL_SCREEN)
        _initialize_hagl_display();
    #endif
      
    return _display;
}

void deinitialize_display() {
    delete _display; _display = NULL;

    #if defined(EPAPER_DISPLAY)
        free(_gdew27w3tDisplay); _gdew27w3tDisplay = NULL;
        free(_spi_io_interface); _spi_io_interface = NULL;
        free(_touch_panel);      _touch_panel = NULL;
    #endif
}
