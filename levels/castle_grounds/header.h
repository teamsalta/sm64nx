#ifndef CASTLE_GROUNDS_HEADER_H
#define CASTLE_GROUNDS_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(castle_grounds_geo_000660);
EXTERN_GeoLayout(castle_grounds_geo_0006F4);
EXTERN_GeoLayout(castle_grounds_geo_00070C);
EXTERN_GeoLayout(castle_grounds_geo_000724);
EXTERN_GeoLayout(castle_grounds_level_geo);

// leveldata
EXTERN_Gfx(castle_grounds_seg7_dl_07006D70);
EXTERN_Gfx(castle_grounds_seg7_dl_070095F0);
EXTERN_Gfx(castle_grounds_seg7_dl_0700A290);
EXTERN_Gfx(castle_grounds_seg7_dl_0700A860);
EXTERN_Gfx(castle_grounds_seg7_dl_0700B1D0);
EXTERN_Gfx(castle_grounds_seg7_dl_0700BA20);
EXTERN_Gfx(castle_grounds_seg7_dl_0700BB80);
EXTERN_Gfx(castle_grounds_seg7_dl_0700BC68);
EXTERN_Gfx(castle_grounds_seg7_dl_0700C210);
EXTERN_Gfx(castle_grounds_seg7_dl_0700C430);
EXTERN_Gfx(castle_grounds_seg7_dl_0700C670);
EXTERN_Gfx(castle_grounds_seg7_dl_0700C6A8);
EXTERN_Gfx(castle_grounds_seg7_dl_0700C6E8);
EXTERN_Gfx(castle_grounds_seg7_dl_0700C728);
EXTERN_Gfx(castle_grounds_seg7_dl_0700C768);
extern const Animation *const castle_grounds_seg7_anims_flags[];
EXTERN_Gfx(castle_grounds_seg7_dl_0700EA58);
EXTERN_Gfx(castle_grounds_seg7_us_dl_0700F2E8);
EXTERN_Collision(castle_grounds_seg7_collision_level);
EXTERN_MacroObject(castle_grounds_seg7_macro_objs);
EXTERN_Collision(castle_grounds_seg7_collision_moat_grills);
EXTERN_Collision(castle_grounds_seg7_collision_cannon_grill);
EXTERN_MovtexQuadCollection(castle_grounds_movtex_water);
extern Movtex castle_grounds_movtex_tris_waterfall[];
EXTERN_Gfx(castle_grounds_dl_waterfall);

// script
EXTERN_LevelScript(level_castle_grounds_entry);

#endif
