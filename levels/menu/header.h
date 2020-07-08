#ifndef MENU_HEADER_H
#define MENU_HEADER_H

#include "types.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(geo_menu_mario_save_button);
EXTERN_GeoLayout(geo_menu_mario_save_button_fade);
EXTERN_GeoLayout(geo_menu_mario_new_button);
EXTERN_GeoLayout(geo_menu_mario_new_button_fade);
EXTERN_GeoLayout(geo_menu_erase_button);
EXTERN_GeoLayout(geo_menu_copy_button);
EXTERN_GeoLayout(geo_menu_file_button);
EXTERN_GeoLayout(geo_menu_score_button);
EXTERN_GeoLayout(geo_menu_sound_button);
EXTERN_GeoLayout(geo_menu_generic_button);
EXTERN_GeoLayout(menu_file_select_strings_and_menu_cursor_level_geo);
EXTERN_GeoLayout(geo_menu_act_selector_strings);

// leveldata
EXTERN_Gfx(dl_menu_mario_save_button_base);
EXTERN_Gfx(dl_menu_mario_new_button_base);
EXTERN_Gfx(dl_menu_save_button_back);
EXTERN_Gfx(dl_menu_save_button_fade_back);
EXTERN_Gfx(dl_menu_erase_button);
EXTERN_Gfx(dl_menu_copy_button);
EXTERN_Gfx(dl_menu_file_button);
EXTERN_Gfx(dl_menu_score_button);
EXTERN_Gfx(dl_menu_sound_button);
EXTERN_Gfx(dl_menu_generic_button);
EXTERN_Gfx(dl_menu_idle_hand);
EXTERN_Gfx(dl_menu_grabbing_hand);
EXTERN_u8(*const menu_hud_lut);
EXTERN_u8(*const menu_font_lut);
EXTERN_Gfx(dl_menu_ia8_text_begin);
EXTERN_Gfx(dl_menu_ia8_text_end);
EXTERN_Gfx(dl_menu_rgba16_wood_course);
EXTERN_Gfx(dl_menu_texture_course_upper);
EXTERN_Gfx(dl_menu_texture_niveau_upper);
EXTERN_Gfx(dl_menu_texture_kurs_upper);
EXTERN_Collision(main_menu_seg7_collision);

// script
EXTERN_LevelScript(level_main_menu_entry_1);
EXTERN_LevelScript(level_main_menu_entry_2);

#endif
