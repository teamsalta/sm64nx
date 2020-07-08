#ifndef SL_HEADER_H
#define SL_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(sl_geo_000360);
EXTERN_GeoLayout(sl_geo_000378);
EXTERN_GeoLayout(sl_geo_000390);
EXTERN_GeoLayout(sl_level_geo);
EXTERN_GeoLayout(sl_underground_level_geo);

// leveldata
EXTERN_Gfx(sl_seg7_dl_07005478);
EXTERN_Gfx(sl_seg7_dl_070056B0);
EXTERN_Gfx(sl_seg7_dl_070073D0);
EXTERN_Gfx(sl_seg7_dl_07007880);
EXTERN_Gfx(sl_seg7_dl_070088B0);
EXTERN_Gfx(sl_seg7_dl_07008D58);
EXTERN_Gfx(sl_seg7_dl_0700A5A0);
EXTERN_Gfx(sl_seg7_dl_0700A780);
EXTERN_Gfx(sl_seg7_dl_0700A890);
EXTERN_Gfx(sl_seg7_dl_0700A980);
EXTERN_Gfx(sl_seg7_dl_0700BAE8);
EXTERN_Gfx(sl_seg7_dl_0700BCF8);
EXTERN_Gfx(sl_seg7_dl_0700C9E8);
EXTERN_Gfx(sl_seg7_dl_0700CB58);
EXTERN_Collision(sl_seg7_area_1_collision);
EXTERN_MacroObject(sl_seg7_area_1_macro_objs);
EXTERN_Collision(sl_seg7_collision_sliding_snow_mound);
EXTERN_Collision(sl_seg7_collision_pound_explodes);
EXTERN_Collision(sl_seg7_area_2_collision);
EXTERN_MacroObject(sl_seg7_area_2_macro_objs);
EXTERN_MovtexQuadCollection(sl_movtex_water);

// script
EXTERN_LevelScript(level_sl_entry);

#endif
