#ifndef _ZUBE_INFO_HUD
#define _ZUBE_INFO_HUD

#include "stdint.h"

void info_hud_initialize();
void info_hud_deinitialize();
void info_hud_draw_background(const char* hex_string);

void info_hud_update_bottom(const char* text, uint8_t text_length);
void info_hud_update_top(const char* text, uint8_t text_length);


#endif /* _ZUBE_INFO_HUD */