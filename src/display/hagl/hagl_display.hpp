#pragma once
#include "../display.hpp"

#define FONT_WIDTH 6
#define FONT_HEIGHT 9
#define SCREEN_PIXEL_BUFFER_SIZE 0

class HaglDisplay final : public IDisplay {
    private:
        static const unsigned char *_font;
        static const Size2D _font_size;
        // this heads up display doesn't buffer, yet needs it to implement the interface
        uint8_t _pixel_buffer[SCREEN_PIXEL_BUFFER_SIZE];
        Size2D _displaySize;

    public:
        HaglDisplay(Size2D displaySize);
        ~HaglDisplay();
        const Size2D getScreenSize() override;
        const Position2D getScreenCenter() override;
        const Size2D getFontSize(const uint8_t font_size_factor) override;
        void drawText(const char* label, uint16_t label_length, Position2D position, uint8_t font_size_factor) override;
        void drawFastVLine(const int x, const int y, const int width, uint16_t color) override;
        void drawFastHLine(const int x, const int y, const int height, uint16_t color) override;
        void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) override;
        void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) override;
        void fillScreen(const uint16_t color) override;
        uint32_t getScreenPixelBufferSize() override;
        const uint8_t* getScreenPixelBuffer() override;
        void flushUpdates() override;
};