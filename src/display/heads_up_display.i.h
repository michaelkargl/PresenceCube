#pragma once

class IHeadsUpDisplay {
    public:
        virtual void drawBackground() = 0;
        virtual void updateCenterLabel(const char* label, uint16_t label_length) = 0;
        virtual void updateCenterValue(const char* text, uint16_t text_length) = 0;
        virtual void updateBottomText(const char* text, uint16_t text_length) = 0;
        virtual void updateTopText(const char* text, uint16_t text_length) = 0;
        virtual void flushUpdates() = 0;
};
