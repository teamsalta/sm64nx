#ifndef THI_HEADER_H
#define THI_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(thi_geo_0005B0);
EXTERN_GeoLayout(thi_geo_0005C8);
EXTERN_GeoLayout(thi_geo_0005F0);
EXTERN_GeoLayout(thi_level_geo_000608);
EXTERN_GeoLayout(thi_level_geo_0006D4);
EXTERN_GeoLayout(thi_level_geo_00079C);

// leveldata
EXTERN_Gfx(thi_seg7_dl_07005260);
EXTERN_Gfx(thi_seg7_dl_07006968);
EXTERN_Gfx(thi_seg7_dl_07007008);
EXTERN_Gfx(thi_seg7_dl_070072E8);
EXTERN_Gfx(thi_seg7_dl_070073C0);
EXTERN_Gfx(thi_seg7_dl_07007538);
EXTERN_Gfx(thi_seg7_dl_07007648);
EXTERN_Gfx(thi_seg7_dl_07007930);
EXTERN_Gfx(thi_seg7_dl_07007C20);
EXTERN_Gfx(thi_seg7_dl_07009670);
EXTERN_Gfx(thi_seg7_dl_07009D50);
EXTERN_Gfx(thi_seg7_dl_07009F58);
EXTERN_Collision(thi_seg7_area_1_collision);
EXTERN_Collision(thi_seg7_area_2_collision);
EXTERN_Collision(thi_seg7_area_3_collision);
EXTERN_MacroObject(thi_seg7_area_1_macro_objs);
EXTERN_MacroObject(thi_seg7_area_2_macro_objs);
EXTERN_MacroObject(thi_seg7_area_3_macro_objs);
EXTERN_Collision(thi_seg7_collision_top_trap);
EXTERN_Trajectory(thi_seg7_trajectory_koopa);
EXTERN_MovtexQuadCollection(thi_movtex_area1_water);
EXTERN_MovtexQuadCollection(thi_movtex_area2_water);

// script
EXTERN_LevelScript(level_thi_entry);

#endif
