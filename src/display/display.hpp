#pragma once
#include "stdint.h"
#include "position_2d.hpp"
#include "size_2d.hpp"


// TODO: Font size is highly dependent on the display and its framework => move to concrete implementations
#define BASE_FONT_SIZE_X 6
#define BASE_FONT_SIZE_Y 8

class IDisplay {
    public:
        const Size2D base_font_size = { BASE_FONT_SIZE_X, BASE_FONT_SIZE_Y };
        virtual ~IDisplay() { };
        virtual const Size2D getScreenSize() = 0;
        virtual const Position2D getScreenCenter() = 0;
        
        /**
         * @brief Returns the scaled font size
         * @param font_size_factor Multiplication factor starting at 1 for default 6x8 font size. 2 is 12x16, 3 is 18x24, etc
         * @return const Size2D
         */
        virtual const Size2D getFontSize(const uint8_t font_size_factor) = 0;
        
        /**
         * @brief Draws a given string to the display
         * @param label Text to draw to the screen
         * @param label_length The text lenght including null terminator
         * @param position The position at which to draw the text to (top-left origin)
         * @param font_size_factor Multiplication factor starting at 1 for default 6x8 font size. 2 is 12x16, 3 is 18x24, etc
         */
        virtual void drawText(const char* label, uint16_t label_length, Position2D position, uint8_t font_size_factor) = 0;
        
        /**
         * @brief Draw a perfectly horizontal line
         * @param x     Left-most x coordinate
         * @param y     Left-most y coordinate
         * @param width Width in pixels
         * @param color 16-bit 5-6-5 Color to fill with
        */
        virtual void drawFastVLine(const int x, const int y, const int width, uint16_t color) = 0;

        /**
         * @brief Draw a perfectly vertical line
         * @param x      Top-most x coordinate
         * @param y      Top-most y coordinate
         * @param height Height in pixels
         * @param color  16-bit 5-6-5 Color to fill with
         */
        virtual void drawFastHLine(const int x, const int y, const int height, uint16_t color) = 0;

        /**
         * Draw a circle
         *
         * Output will be clipped to the current clip window.
         *
         * @param x0 center X
         * @param y0 center Y
         * @param r radius
         * @param color
         */
        virtual void drawCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color) = 0;
        
        /**
         *  @brief Fill a rectangle completely with one color
         *  @param x     Top left corner x coordinate
         *  @param y     Top left corner y coordinate
         *  @param w     Width in pixels
         *  @param h     Height in pixels
         *  @param color 16-bit 5-6-5 Color to fill with
         */
        virtual void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) = 0;
        /**
         * @brief Fills the entire screen
         * @param color color to fill the screen with
         */
        virtual void fillScreen(const uint16_t color);

        /**
         * @brief Get the screens pixel buffer where each entry describes the color of the pixel
         */
        virtual uint32_t getScreenPixelBufferSize() = 0;

        /**
         * @brief Gets the screens pixel buffer array. The pixel buffer is a digital representation of the 
         * screen contents where each pixel is assigned a color.
         * For example:buffer[x * y] = 255 for black pixel.
         */
        virtual const uint8_t * getScreenPixelBuffer() = 0;

        /**
         * @brief Applies queued changes and sends paint commands to the physical display.
        */
        virtual void flushUpdates() = 0;

};


