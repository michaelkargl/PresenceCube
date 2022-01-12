#include "display.hpp"

class EpaperDisplay final : public IDisplay {
    private:
        Epd* display = NULL;
    public:
        EpaperDisplay(Epd* display) : display(display) {}
    
    void drawText(
        const char* label, 
        uint16_t label_length, 
        Position2D position, 
        uint8_t font_size_factor
    ) {
        this->display->setCursor(position.x, position.y);
        this->display->setTextColor(EPD_BLACK);
        this->display->setTextSize(font_size_factor);
        this->display->setTextWrap(true);
        this->display->println(label);
    }

    void drawFastVLine(const int x, const int y, const int width, uint16_t color) {
        this->display->drawFastVLine(x, y, width, color);
    }
    
    void drawFastHLine(const int x, const int y, const int height, uint16_t color) {
        this->display->drawFastHLine(x, y, height, color);
    }
    
    const Position2D getScreenCenter() {
        const Size2D screen_size = this->getScreenSize();
        const Position2D center = {
            screen_size.x / 2,
            screen_size.y / 2
        };

        return center;
    }

    const Size2D getScreenSize() {
        const Size2D size = {
            this->display->width(),
            this->display->height()
        };

        return size;
    }

    const Size2D getFontSize(const uint8_t font_size_factor) {
        Size2D font_size = {
            this->base_font_size.x * font_size_factor,
            this->base_font_size.y * font_size_factor
        };

        return font_size;
    }

    void fillScreen(const uint16_t color) {
        this->display->fillScreen(color);
    }

    void flushUpdates() {
        this->display->update();
    }
};