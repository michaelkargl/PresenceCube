#include "st7789.h"
#include "esp_vfs.h"
#include "display.h"
#include "esp_system.h"

static TFT_t _display;
static FontxFile _font[2];


// Proxy to not rely too heavily on ESP functionality
static uint32_t _random_number() {
    return esp_random();
}

static void _initialize_font(const char* font_path) {
    
	InitFontx(_font, font_path, ""); // 16x32Dot Gothic

	// sizes initialized on open =>
	OpenFontx(&_font[0]);
	CloseFontx(&_font[0]);
}


void initialize_display(
    int16_t gpio_mosi,
    int16_t gpio_sclk,
    int16_t gpio_cs,
    int16_t gpio_dc,
    int16_t gpio_reset,
    int16_t gpio_bl,
    int16_t size_x,
    int16_t size_y,
    int16_t offset_x,
    int16_t offset_y,
    const char* font_path
) {
    spi_master_init(
        &_display,
        gpio_mosi,
        gpio_sclk,
        gpio_cs,
        gpio_dc,
        gpio_reset,
        gpio_bl);
	lcdInit(
		&_display,
        size_x,
        size_y,
        offset_x,
        offset_y);

    lcdInversionOn(&_display);
    _initialize_font(font_path);
}

uint16_t get_random_size() {
    const uint8_t scale_down_factor = 5;
    return _random_number() % (_display._width/scale_down_factor);
}

vector2d_t get_random_position() {
    const vector2d_t position = {
        .x = _random_number() % _display._width,
        .y = _random_number() % _display._height
    };
    
    return position;
}

rgb565_t get_rgb565color(uint16_t red, uint16_t green, uint16_t blue) {
    return rgb565_conv(red, green, blue);
}

rgb565_t get_rgb565color_(color_t color) {
    return get_rgb565color(color.r, color.g, color.b);
}

rgb565_t get_random_color() {
	const color_t color = {
        .r = _random_number() % 255,
        .g = _random_number() % 255,
        .b = _random_number() % 255
    };
    
    return get_rgb565color_(color);
}

void draw_string(
    uint16_t position_x, 
    uint16_t position_y, 
    rgb565_t color_foreground,
    rgb565_t color_background,
    uint8_t* text) {
    
    lcdSetFontFill(&_display, color_background);
    lcdDrawString(
        &_display, 
        _font, 
        position_x, 
        position_y, 
        text,
        color_foreground);
}

void display_random_rectangle() {
    const rgb565_t color = get_random_color();
    const vector2d_t position = get_random_position();
    const uint16_t size = get_random_size();
    
    draw_square(position.x, position.y, size, color);
}

void fill_screen(const rgb565_t color) {
    lcdFillScreen(&_display, color);
}

void draw_square(
    const uint16_t position_x, 
    const uint16_t position_y, 
    const uint16_t size, 
    const rgb565_t color) {
    draw_rectangle(
        position_x, 
        position_y,
        size,
        size,
        color
    );
}

void draw_rectangle(
    const uint16_t position_x, 
    const uint16_t position_y, 
    const uint16_t size_x, 
    const uint16_t size_y, 
    const rgb565_t color) {

    const uint16_t position_x2 = position_x + size_x;
    const uint16_t position_y2 = position_y + size_y;    
    lcdDrawFillRect(
        &_display, 
        position_x, position_y, 
        position_x2, position_y2, 
        color);
}