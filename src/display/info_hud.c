#include "info_hud.h"
#include "hagl.h"
#include "hagl/hagl_extensions.h"


#include "font6x9.h"
#define FONT font6x9
#define FONT_WIDTH 6
#define FONT_HEIGHT 9
#define FONT_SCALING 6
#define WHITE 0xFFFF


void info_hud_initialize()
{
    hagl_init();
}

void info_hud_deinitialize()
{
    hagl_flush();
    hagl_close();
}


void info_hud_draw_background(const char* hex_string)
{
    const uint16_t center_x = DISPLAY_WIDTH / 2;
    const uint16_t center_y = DISPLAY_HEIGHT / 2;
    const color_t color = hagl_color_from_hex_string(hex_string);

    for (uint16_t i = 1; i < DISPLAY_HEIGHT; i += 1)
    {
        hagl_draw_circle(center_x, center_y, i, color);
        hagl_flush();
    }
}

void info_hud_update_top(const char *text, uint8_t text_length)
{
    const uint16_t x = 20;
    const uint16_t y = 10;
    draw_scaled_text(
        text,
        text_length,
        x,
        y,
        WHITE,
        FONT,
        FONT_WIDTH,
        FONT_HEIGHT,
        FONT_SCALING);
}

void info_hud_update_bottom(const char *text, uint8_t text_length)
{
    const uint16_t x = 20;
    const uint16_t top_hud_item_size = FONT_HEIGHT * FONT_SCALING;
    const uint8_t margin_top = 5;
    const uint16_t y = top_hud_item_size + margin_top;
    draw_scaled_text(
        text,
        text_length,
        x,
        y,
        WHITE,
        FONT,
        FONT_WIDTH,
        FONT_HEIGHT,
        FONT_SCALING);
}
