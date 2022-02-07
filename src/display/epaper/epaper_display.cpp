#include "epaper_display.hpp"
#include "gdew_colors.h"

#define SCREEN_PIXEL_BUFFER_SIZE 0


void EpaperDisplay::drawText(
    const char* label,
    uint16_t label_length, 
    Position2D position, 
    uint8_t font_size_factor
) {
    this->display->setCursor(position.x, position.y);
    this->display->setTextColor(EPD_BLACK, EPD_WHITE);
    this->display->setTextSize(font_size_factor);
    this->display->setTextWrap(true);
    this->display->println(label);
}

void EpaperDisplay::drawFastVLine(const int x, const int y, const int width, uint16_t color) {
    this->display->drawFastVLine(x, y, width, color);
}
    
void EpaperDisplay::drawFastHLine(const int x, const int y, const int height, uint16_t color) {
    this->display->drawFastHLine(x, y, height, color);
}

void EpaperDisplay::fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) {
    this->display->fillRect(x, y, width, height, color);
}

// TODO: move to base class
const Size2D EpaperDisplay::getFontSize(const uint8_t font_size_factor) {
    Size2D font_size = {
        this->base_font_size.x * font_size_factor,
        this->base_font_size.y * font_size_factor
    };

    return font_size;
}

void EpaperDisplay::fillScreen(const uint16_t color) {
    this->display->fillScreen(color);
}

uint32_t EpaperDisplay::getScreenPixelBufferSize() {
    return sizeof(this->_pixel_buffer) / sizeof(this->_pixel_buffer[0]);
}

const uint8_t* EpaperDisplay::getScreenPixelBuffer() {
    return this->_pixel_buffer;
}

void EpaperDisplay::flushUpdates() {
    this->display->update();
}

const Size2D EpaperDisplay::EpaperDisplay::getScreenSize() {
    const Size2D size = {
        this->display->width(),
        this->display->height()
    };

    return size;
}

const Position2D EpaperDisplay::EpaperDisplay::getScreenCenter() {
    const Size2D screen_size = this->getScreenSize();
    const Position2D center = {
        screen_size.x / 2,
        screen_size.y / 2
    };

    return center;
}
