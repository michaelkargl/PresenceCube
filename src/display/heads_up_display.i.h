#pragma once
#include "stdint.h"

class IHeadsUpDisplay {
    public:
        virtual void drawBackground(uint16_t color) = 0;
        virtual void updateCenterLabel(const char* label, uint16_t label_length) = 0;
        virtual void updateCenterValue(const char* text, uint16_t text_length) = 0;
        virtual void updateBottomText(const char* text, uint16_t text_length) = 0;
        virtual void updateTopText(const char* text, uint16_t text_length) = 0;
        virtual const uint8_t* getScreenPixels() = 0;
        virtual const uint32_t getScreenPixelSize() = 0;
        virtual const Size2D getScreenSize() = 0;
        virtual void flushUpdates() = 0;
};
