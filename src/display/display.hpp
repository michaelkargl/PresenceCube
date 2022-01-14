#pragma once
#include "stdint.h"
#include "position_2d.hpp"
#include "size_2d.hpp"
#include "epd.h"
#include "gdew_colors.h"

#define BASE_FONT_SIZE_X 6
#define BASE_FONT_SIZE_Y 8

class IDisplay {
    public:
        const Size2D base_font_size = { BASE_FONT_SIZE_X, BASE_FONT_SIZE_Y };
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
         * @brief Applies queued changes and sends paint commands to the physical display.
        */
        virtual void flushUpdates() = 0;

};


