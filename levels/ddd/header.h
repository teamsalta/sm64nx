#ifndef DDD_HEADER_H
#define DDD_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(ddd_geo_000450);
EXTERN_GeoLayout(ddd_geo_000478);
EXTERN_GeoLayout(ddd_geo_0004A0);
EXTERN_GeoLayout(ddd_geo_0004C0);
EXTERN_GeoLayout(ddd_level_geo);

// leveldata
EXTERN_Gfx(ddd_seg7_dl_07004D48);
EXTERN_Gfx(ddd_seg7_dl_070057E8);
EXTERN_Gfx(ddd_seg7_dl_07005C40);
EXTERN_Gfx(ddd_seg7_dl_07005F78);
EXTERN_Gfx(ddd_seg7_dl_07007408);
EXTERN_Gfx(ddd_seg7_dl_07007CB8);
EXTERN_Gfx(ddd_seg7_dl_07008C48);
EXTERN_Gfx(ddd_seg7_dl_07008F80);
EXTERN_Gfx(ddd_seg7_dl_07009030);
EXTERN_Gfx(ddd_seg7_dl_07009120);
EXTERN_Gfx(ddd_seg7_dl_07009208);
EXTERN_Gfx(ddd_seg7_dl_0700AF10);
EXTERN_Gfx(ddd_seg7_dl_0700B068);
EXTERN_Gfx(ddd_seg7_dl_0700BAE0);
EXTERN_Gfx(ddd_seg7_dl_0700CE48);
EXTERN_Gfx(ddd_seg7_dl_0700D2A0);
EXTERN_Collision(ddd_seg7_area_1_collision);
EXTERN_Collision(ddd_seg7_area_2_collision);
EXTERN_MacroObject(ddd_seg7_area_1_macro_objs);
EXTERN_MacroObject(ddd_seg7_area_2_macro_objs);
EXTERN_Collision(ddd_seg7_collision_submarine);
EXTERN_Collision(ddd_seg7_collision_bowser_sub_door);
EXTERN_MovtexQuadCollection(ddd_movtex_area1_water);
EXTERN_MovtexQuadCollection(ddd_movtex_area2_water);

// script
EXTERN_LevelScript(level_ddd_entry);

#endif
