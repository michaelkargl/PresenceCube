#pragma once
#include "heads_up_display.i.h"
#include "esp_log.h"
#include "position_2d.hpp"
#include "size_2d.hpp"
#include "display.hpp"

#define TOP_LABEL_FONT_SIZE_FACTOR 2
#define BOTTOM_LABEL_FONT_SIZE_FACTOR 2
#define CENTER_LABEL_FONT_FIZE_FACTOR 3
#define CENTER_VALUE_FONT_SIZE_FACTOR 8
#define CENTER_LABEL_MARGIN_BOTTOM 40
#define CENTER_VALUE_MARGIN_TOP 15
#define BOTTOM_LABEL_MARGIN_BOTTOM 5

// TODO: #55 Redesign HeadUpDisplay
class HeadsUpDisplay : public IHeadsUpDisplay {

    private:
        const char* _loggingTag = "HeadsUpDisplay";
        IDisplay* _display;
        bool _is_dirty = false;

        const Position2D getBottomTextPosition(
            const uint8_t font_size_factor, 
            uint32_t label_length
        );
        const Position2D getTopTextPosition(
            const uint8_t font_size_factor,
            const uint16_t text_length
        );
        const Position2D getCenterValuePosition(
            const uint8_t font_size_factor, 
            const uint16_t text_length
        );
        const Position2D getCenterLabelPosition(
            const uint8_t font_size_factor, 
            const uint32_t label_length
        );
        const Position2D getTextCenterOrigin(
            const uint8_t font_size_factor,
            const uint32_t text_length
        );
        void clearSection(
            const uint8_t font_size_factor, 
            const Position2D elementPosition
        );
        void setDirtyFlag(bool flag);
        void markScreenDirty();
        void markScreenClean();
    
    public:
        HeadsUpDisplay(IDisplay* display): _display(display) {}
        const uint8_t* getScreenPixels() override;
        const uint32_t getScreenPixelSize() override;
        const Size2D getScreenSize() override;
        void drawBackground(uint16_t color) override;

    /**
     * @brief Updates the label displayed at the center
     * @param label The label to display at the center
     * @param label_length The size of the label including \0
     * @param font_size_factor Multiplication factor starting at 1 for default 6x8 font size. 2 is 12x16, 3 is 18x24, etc
     */
    void updateCenterLabel(const char* label, uint16_t label_length) {
        Position2D position = this->getCenterLabelPosition(CENTER_LABEL_FONT_FIZE_FACTOR, label_length);

        this->clearSection(CENTER_LABEL_FONT_FIZE_FACTOR, position);
        this->_display->drawText(label, label_length, position, CENTER_LABEL_FONT_FIZE_FACTOR);
        this->markScreenDirty();
    }

    /**
     * @brief Updates the center value
     * @param text The text to render
     * @param text_length The text length
     */
    void updateCenterValue(const char* text, uint16_t text_length) {
        ESP_LOGI(this->_loggingTag, "Updating center value with length %i: %s", text_length, text);
        Position2D position = this->getCenterValuePosition(CENTER_VALUE_FONT_SIZE_FACTOR, text_length);

        this->clearSection(CENTER_VALUE_FONT_SIZE_FACTOR, position);
        this->_display->drawText(text, text_length, position, CENTER_VALUE_FONT_SIZE_FACTOR);
        this->markScreenDirty();
    }

    /**
     * @brief Updates the text at the bottom
     * @param text The text to render
     * @param text_length The text length
     */
    void updateBottomText(const char* text, uint16_t text_length) {
        Position2D position = this->getBottomTextPosition(BOTTOM_LABEL_FONT_SIZE_FACTOR, text_length);
        
        this->clearSection(BOTTOM_LABEL_FONT_SIZE_FACTOR, position);
        this->_display->drawText(text, text_length, position, BOTTOM_LABEL_FONT_SIZE_FACTOR);
        this->markScreenDirty();
    }

    /**
     * @brief Updates the text at the top
     * 
     * @param text The text to render
     * @param text_length The length of the text
     */
    void updateTopText(const char* text, uint16_t text_length) {
        Position2D position = this->getTopTextPosition(TOP_LABEL_FONT_SIZE_FACTOR, text_length);

        this->clearSection(TOP_LABEL_FONT_SIZE_FACTOR, position);
        this->_display->drawText(text, text_length, position, TOP_LABEL_FONT_SIZE_FACTOR);
        this->markScreenDirty();
    }

    /**
     * @brief Applies queued changes and sends paint commands to the physical display.
     */
    void flushUpdates() {
        if ( this->_is_dirty) {
            _display->flushUpdates();
            this->markScreenClean();
        } else {
            ESP_LOGD(this->_loggingTag, "No updates are queued, ignoring flush request...");
        }
    }
};

/**
 * @brief Get the texts center origin. Assuming a top-left origin of 0,0
 * @param font_size_factor Multiplication factor starting at 1 for default 6x8 font size. 2 is 12x16, 3 is 18x24, etc
 * @param text_length The text length
 * @return const Position2D The position at the very center of specified text.
 * @pre    o-----------------------+    +-----------------------+
 * @pre    |                       | => |           o           |
 * @pre    +-----------------------+    +-----------------------+
 */
const Position2D HeadsUpDisplay::getTextCenterOrigin(
    const uint8_t font_size_factor,
    const uint32_t text_length
) {
    Size2D font_size = this->_display->getFontSize(font_size_factor);
    Size2D text_size_in_pixel = {
        (int)text_length * font_size.x,
        font_size.y
    };

    Position2D center_origin = {
        text_size_in_pixel.x / 2,
        text_size_in_pixel.y / 2
    };

    ESP_LOGD(
        this->_loggingTag, 
        "Calculated center origin of font size (%i): (%i, %i)",
        font_size_factor, center_origin.x, center_origin.y
    );
    return center_origin;
}

/**
 * @brief Gets the position of the top text field
 * @param font_size_factor Multiplication factor starting at 1 for default 6x8 font size. 2 is 12x16, 3 is 18x24, etc
 * @param text_length The text length to calculate the text position for 
 * @return const Position2D 
 */
const Position2D HeadsUpDisplay::getTopTextPosition(
    const uint8_t font_size_factor,
    const uint16_t text_length
) {
    Size2D font_size = this->_display->getFontSize(font_size_factor);
    Position2D screen_center = this->_display->getScreenCenter();
    Position2D text_center_origin = this->getTextCenterOrigin(font_size_factor, text_length);

    Position2D position = {
        screen_center.x - text_center_origin.x,
        font_size.y - text_center_origin.y
    };

    return position;
}

/**
 * @brief Gets the position for the text at the bottom
 * @param font_size_factor Multiplication factor starting at 1 for default 6x8 font size. 2 is 12x16, 3 is 18x24, etc
 * @return Position2D
 */
const Position2D HeadsUpDisplay::getBottomTextPosition(
    const uint8_t font_size_factor, 
    uint32_t text_length
) {
    const Size2D font_size = this->_display->getFontSize(font_size_factor);
    const Position2D text_center_origin = this->getTextCenterOrigin(font_size_factor, text_length);
    const Size2D screen_size = this->getScreenSize();
    const Position2D screen_center = this->_display->getScreenCenter();
    
    const Position2D position {
        screen_center.x - text_center_origin.x,
        screen_size.y - font_size.y - BOTTOM_LABEL_MARGIN_BOTTOM
    };

    return position;
}

/**
 * @brief 
 * @param font_size_factor Multiplication factor starting at 1 for default 6x8 font size. 2 is 12x16, 3 is 18x24, etc
 * @param label_length The length of the label
 * @return Position2D 
 */
const Position2D HeadsUpDisplay::getCenterLabelPosition(const uint8_t font_size_factor, uint32_t label_length) {
    const Position2D screen_center = this->_display->getScreenCenter();
    Position2D label_center_origin = this->getTextCenterOrigin(font_size_factor, label_length);
    
    // shift text center to screen center
    // +-------------+    +--------------+
    // |  o          |    |  o----+      |
    // |             |    |       |      |
    // |             | => |       +      |
    // |             |    |              |
    // |             |    |              |
    // +-------------+    +--------------+
    const Position2D text_position {
        screen_center.x - label_center_origin.x,
        screen_center.y - label_center_origin.y - CENTER_LABEL_MARGIN_BOTTOM
    };

    return text_position;
}


/**
 * @brief Gets the text position for the center value
 * @param font_size_factor The size of the font to render the text in
 * @param text_length The length of the text
 * @return const Position2D
 */
const Position2D HeadsUpDisplay::getCenterValuePosition(
    const uint8_t font_size_factor,
    const uint16_t text_length
) {
    // we only care for the origin y-axis, therefore we can safely assume a string of length 1
    Position2D center_label_position = this->getCenterLabelPosition(CENTER_LABEL_FONT_FIZE_FACTOR, 1);
    Size2D center_label_font_size = this->_display->getFontSize(CENTER_LABEL_FONT_FIZE_FACTOR);

    Position2D center_value_center_origin = this->getCenterLabelPosition(font_size_factor, text_length);
    
    Position2D screen_center = this->_display->getScreenCenter();

    // centered text right below label
    const Position2D position = {
        screen_center.x - center_value_center_origin.x,
        center_label_position.y + center_label_font_size.y + CENTER_VALUE_MARGIN_TOP
    };

    return position;
}


/**
 * @brief Prepares / clears the entire section so that new values can be drawn.
 *        Without this step, artifacts may appear when updating texts with smaller values.
 * 
 * @param font_size_factor font size of the text to be drawn
 * @param elementPosition position of the element to be drawn
 */
void HeadsUpDisplay::clearSection(const uint8_t font_size_factor, const Position2D elementPosition) {
    Size2D font_size = this->_display->getFontSize(BOTTOM_LABEL_FONT_SIZE_FACTOR);
    Size2D screen_size = this->getScreenSize();
    Position2D sectionPosition = { 0, elementPosition.y };
    Size2D sectionSize = { screen_size.x, font_size.y };

    const uint16_t white = 0x0000;
    this->_display->fillRectangle(
        sectionPosition.x, sectionPosition.y,
        sectionSize.x, sectionSize.y, 
        white
    );

    this->markScreenDirty();
}

void HeadsUpDisplay::drawBackground(uint16_t color)
    {
    ESP_LOGI(this->_loggingTag, "Drawing circles...");
    const Size2D screen_size = this->getScreenSize();
    Position2D center = this->_display->getScreenCenter();
    
    const uint8_t circle_count = 20;
    
    for (uint16_t i = 1; i < circle_count; i++) {    
        int16_t radius = i * (screen_size.x / circle_count);
        this->_display->drawCircle(center.x, center.y, radius, color);
    }
    this->markScreenDirty();
}

void HeadsUpDisplay::setDirtyFlag(bool flag) {
    this->_is_dirty = flag;
}

/**
 * @brief Marks the screen as dirty to know when updates are needed.
 */
void HeadsUpDisplay::markScreenDirty() {
    this->_is_dirty = true;
}

/**
 * @brief Resets the screens dirty flag to know when to ignore unnecessary screen updates.
 */
void HeadsUpDisplay::markScreenClean() {
    this->_is_dirty = false;
}

const uint8_t* HeadsUpDisplay::getScreenPixels() {
    return this->_display->getScreenPixelBuffer();
}

const uint32_t HeadsUpDisplay::getScreenPixelSize() {
    return this->_display->getScreenPixelBufferSize();
}

const Size2D HeadsUpDisplay::getScreenSize() {
    return this->_display->getScreenSize();
}