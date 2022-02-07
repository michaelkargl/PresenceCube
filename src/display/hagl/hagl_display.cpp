#include "hagl_display.hpp"
#include "string"
#include "codecvt"
#include <locale>
#include "hagl.h"
#include "font6x9.h"


HaglDisplay::HaglDisplay(Size2D displaySize) : _displaySize(displaySize) {
    hagl_init();
}

HaglDisplay::~HaglDisplay() {
    hagl_flush();
    hagl_close();
}

const unsigned char* HaglDisplay::_font = font6x9;
const Size2D HaglDisplay::_font_size { 6, 9 };


const Size2D HaglDisplay::getScreenSize() { 
    return this->_displaySize; 
}

const Position2D HaglDisplay::getScreenCenter() { 
    const Size2D screen_size = this->getScreenSize();
    const Position2D center = {
        screen_size.x / 2,
        screen_size.y / 2
    };

    return center;
}

// TODO: move to base class
const Size2D HaglDisplay::getFontSize(const uint8_t font_size_factor) {
    Size2D font_size = {
        this->_font_size.x * font_size_factor,
        this->_font_size.y * font_size_factor
    };

    return font_size;
}

void HaglDisplay::drawText(const char* text, uint16_t label_length, Position2D position, uint8_t font_size_factor = 1) {    
    const uint16_t white = 0xffff;

    std::wstring wide_text = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(text);
    hagl_put_text(wide_text.c_str(), position.x, position.y, white, font6x9);
}


void HaglDisplay::drawFastVLine(const int x, const int y, const int height, uint16_t color) {
    hagl_draw_vline(x, y, height, color);
}

void HaglDisplay::drawFastHLine(const int x, const int y, const int width, uint16_t color) {
    hagl_draw_hline(x, y, width, color);
}

void HaglDisplay::drawCircle(int16_t x0, int16_t y0, int16_t radius, color_t color) {
    hagl_draw_circle(x0, y0, radius, color);
}

void HaglDisplay::fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) {
    hagl_draw_rectangle(x, y, x + width, y + height, color);
}

void HaglDisplay::fillScreen(const uint16_t color) {
    hagl_fill_rectangle(0, 0, this->_displaySize.x, this->_displaySize.y, color);
}

uint32_t HaglDisplay::getScreenPixelBufferSize() { 
    return sizeof(this->_pixel_buffer) / sizeof(this->_pixel_buffer[0]);
}

const uint8_t* HaglDisplay::getScreenPixelBuffer() { 
    return this->_pixel_buffer;
}

void HaglDisplay::flushUpdates() {
    hagl_flush();
}