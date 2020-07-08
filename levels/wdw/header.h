#ifndef WDW_HEADER_H
#define WDW_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(wdw_geo_000580);
EXTERN_GeoLayout(wdw_geo_000598);
EXTERN_GeoLayout(wdw_geo_0005C0);
EXTERN_GeoLayout(wdw_geo_0005E8);
EXTERN_GeoLayout(wdw_geo_000610);
EXTERN_GeoLayout(wdw_geo_000628);
EXTERN_GeoLayout(wdw_geo_000640);
EXTERN_GeoLayout(wdw_level_geo);
EXTERN_GeoLayout(wdw_town_level_geo);

// leveldata
EXTERN_Gfx(wdw_seg7_dl_07009AB0);
EXTERN_Gfx(wdw_seg7_dl_0700A138);
EXTERN_Gfx(wdw_seg7_dl_07011E48);
EXTERN_Gfx(wdw_seg7_dl_07012258);
EXTERN_Gfx(wdw_seg7_dl_07012798);
EXTERN_Gfx(wdw_seg7_dl_07012908);
EXTERN_Gfx(wdw_seg7_dl_07012B90);
EXTERN_Gfx(wdw_seg7_dl_07012E88);
EXTERN_Gfx(wdw_seg7_dl_070131B8);
EXTERN_Gfx(wdw_seg7_dl_07013490);
EXTERN_Gfx(wdw_seg7_dl_07013B70);
EXTERN_Gfx(wdw_seg7_dl_07013E40);
EXTERN_Gfx(wdw_seg7_dl_070140E0);
EXTERN_Collision(wdw_seg7_area_1_collision);
EXTERN_MacroObject(wdw_seg7_area_1_macro_objs);
EXTERN_Collision(wdw_seg7_area_2_collision);
EXTERN_MacroObject(wdw_seg7_area_2_macro_objs);
EXTERN_Collision(wdw_seg7_collision_square_floating_platform);
EXTERN_Collision(wdw_seg7_collision_arrow_lift);
EXTERN_Collision(wdw_seg7_collision_070184C8);
EXTERN_Collision(wdw_seg7_collision_07018528);
EXTERN_Collision(wdw_seg7_collision_express_elevator_platform);
EXTERN_Collision(wdw_seg7_collision_rect_floating_platform);
EXTERN_Collision(wdw_seg7_collision_070186B4);
EXTERN_MovtexQuadCollection(wdw_movtex_area1_water);
EXTERN_MovtexQuadCollection(wdw_movtex_area2_water);

// script
EXTERN_LevelScript(level_wdw_entry);

#endif
