#ifndef _HAGL_EXTENSIONS
#define _HAGL_EXTENSIONS

#include "hagl.h"
#include "stdbool.h"


/**
 * @brief parses a rgb hex color to rgb ("FF00FF" => 255,0,255)
 * @param hex_string hex number of length 6 (each channel with leading 0)
 * @param r output red
 * @param g output green
 * @param b output blue
 * @return true if color successfully parsed, otherwise false
 */
bool hagl_hex_string_to_rgb(const char* hex_string, uint8_t *r, uint8_t *g, uint8_t *b);



/**
 * @brief parses a hex color string to a hagl color
 * @param hex_string hex number of length 6 (each channel with leading 0)
 * @return color_t hagl color
 */
color_t hagl_color_from_hex_string(const char* hex_string);


/**
 * @brief returns a random hagl compatible color
 * @return color_t
 */
color_t hagl_color_random();


/**
 * @brief draws the given text enlarged by a certain factor
 * @param text the text to print
 * @param text_length the length of the text to print
 * @param x the top-left x origin of the text to print
 * @param y the top-left y origin of the text to print
 * @param color the rgb888 font color 
 * @param font the FONTX2 font to use
 * @param font_width the width of one font glyph
 * @param font_height the height of one font glyph
 * @param scale_factor the factor by how much the font size should be enlarged
 */
void draw_scaled_text(
    const char* text,
    uint16_t text_length, 
    int x, 
    int y, 
    color_t color,
    const unsigned char *font,
    uint8_t font_width,
    uint8_t font_height,
    uint8_t scale_factor
);

#endif /* _HAGL_EXTENSIONS */