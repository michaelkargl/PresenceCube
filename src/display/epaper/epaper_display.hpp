#pragma once
#include "../display.hpp"
#include "epd.h"

#define SCREEN_PIXEL_BUFFER_SIZE 0

class EpaperDisplay final : public IDisplay {
    private:
        Epd* display = NULL;
        // this heads up display doesn't buffer, yet needs it to implement the interface
        uint8_t _pixel_buffer[SCREEN_PIXEL_BUFFER_SIZE];
    public:
        EpaperDisplay(Epd* display) : display(display) {};
        const Size2D getScreenSize() override;
        const Position2D getScreenCenter() override;
        const Size2D getFontSize(const uint8_t font_size_factor) override;
        void drawText(const char* label, uint16_t label_length, Position2D position, uint8_t font_size_factor) override;
        void drawFastVLine(const int x, const int y, const int width, uint16_t color) override;
        void drawFastHLine(const int x, const int y, const int height, uint16_t color) override;
        void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) override;
        void fillScreen(const uint16_t color) override;
        uint32_t getScreenPixelBufferSize() override;
        const uint8_t* getScreenPixelBuffer() override;
        void flushUpdates() override;
};
