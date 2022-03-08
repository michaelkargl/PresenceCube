#include "hagl_extensions.h"
#include "stdlib.h"
#include "stdio.h"
#include "esp_log.h"

#define _LOGGER_TAG "hagl_extensions"


bool hagl_hex_string_to_rgb(const char* hex_string, uint8_t *r, uint8_t *g, uint8_t *b) {
    const uint8_t expected_number_count = 3;
    // 
    return sscanf(hex_string, "%02hhx%02hhx%02hhx", r, g, b) == expected_number_count;
}


color_t hagl_color_from_hex_string(const char* hex_string) {
    uint8_t r, g, b;

    if ( !hagl_hex_string_to_rgb(hex_string, &r, &g, &b) ){
        r = g = b = 0;
    }
    
    return hagl_color(r, g, b);
}


color_t hagl_color_random() {
    return hagl_color(
        rand() % 0x100,
        rand() % 0x100,
        rand() % 0x100
    );
}



void draw_blitted_character(
    const char character, 
    const uint16_t x, 
    const uint16_t y, 
    const unsigned char *font,
    uint8_t font_size_x,
    uint8_t font_size_y,
    uint8_t new_font_size_x,
    uint8_t new_font_size_y,
    color_t color
) {
    ESP_LOGD(_LOGGER_TAG, "Allocating bitmap buffer");
    bitmap_t bitmap; // dont forget to free mallocated space
    bitmap.buffer = (uint8_t *) malloc(font_size_x * font_size_y * sizeof(color_t));

    ESP_LOGD(_LOGGER_TAG, "Retrieving bitmap glyph for '%c'.", character);
    hagl_get_glyph(character, color, &bitmap, font);

    ESP_LOGD(_LOGGER_TAG, "Scaled rendering");
    hagl_scale_blit(
        x,
        y,
        new_font_size_x,
        new_font_size_y,
        &bitmap);

    hagl_flush();
    free(bitmap.buffer);
}


void draw_scaled_text(
    const char* text,
    uint16_t text_length, 
    int x,
    int y, 
    color_t color,
    const unsigned char *font,
    uint8_t font_size_x,
    uint8_t font_size_y,
    uint8_t scale_factor
) {
    ESP_LOGI(_LOGGER_TAG, "Drawing text '%s'(%i) with scale %i and color %i", text, text_length, scale_factor, color);
    uint8_t scaled_font_width = scale_factor * font_size_x;
    uint8_t scaled_font_height = scale_factor * font_size_y;
    for (uint16_t i = 0; i < text_length; i++) {
        uint16_t char_coordinate_x = x + scaled_font_width * i;
        draw_blitted_character(
            text[i],
            char_coordinate_x,
            y,
            font,
            font_size_x,
            font_size_y,
            scaled_font_width,
            scaled_font_height,
            color
        );        
    }
}