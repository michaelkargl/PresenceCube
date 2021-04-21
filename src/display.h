#pragma once
#include "color_util.h"

typedef struct {
    uint16_t x;
    uint16_t y;
} vector2d_t;

typedef uint16_t rgb565_t;

rgb565_t get_rgb565color(const uint16_t red, const uint16_t green, const uint16_t blue);
rgb565_t get_rgb565color_(const color_t color);
rgb565_t get_random_color();
vector2d_t get_random_position();
uint16_t get_random_size();
void display_random_rectangle();
void fill_screen(const rgb565_t color);
void draw_string(
    uint16_t position_x, 
    uint16_t position_y, 
    rgb565_t color_foreground,
    rgb565_t color_background,
    uint8_t* text);
void draw_rectangle(
    const uint16_t position_x, 
    const uint16_t position_y, 
    const uint16_t size_x, 
    const uint16_t size_y, 
    const rgb565_t color);
void draw_square(
    const uint16_t position_x, 
    const uint16_t position_y, 
    const uint16_t size, 
    const rgb565_t color);
void initialize_display(
    const int16_t gpio_mosi,
    const int16_t gpio_sclk,
    const int16_t gpio_cs,
    const int16_t gpio_dc,
    const int16_t gpio_reset,
    const int16_t gpio_bl,
    const int16_t size_x,
    const int16_t size_y,
    const int16_t offset_x,
    const int16_t offset_y,
    const char* font_path
);