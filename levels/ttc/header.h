#ifndef TTC_HEADER_H
#define TTC_HEADER_H

#include "types.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(ttc_geo_000240);
EXTERN_GeoLayout(ttc_geo_000258);
EXTERN_GeoLayout(ttc_geo_000270);
EXTERN_GeoLayout(ttc_geo_000288);
EXTERN_GeoLayout(ttc_geo_0002A8);
EXTERN_GeoLayout(ttc_geo_0002C8);
EXTERN_GeoLayout(ttc_geo_0002E0);
EXTERN_GeoLayout(ttc_geo_0002F8);
EXTERN_GeoLayout(ttc_geo_000310);
EXTERN_GeoLayout(ttc_geo_000328);
EXTERN_GeoLayout(ttc_geo_000340);
EXTERN_GeoLayout(ttc_geo_000358);
EXTERN_GeoLayout(ttc_geo_000370);
EXTERN_GeoLayout(ttc_geo_000388);
EXTERN_GeoLayout(ttc_geo_0003A0);
EXTERN_GeoLayout(ttc_level_geo);

// leveldata
EXTERN_Gfx(ttc_seg7_dl_0700AD38);
EXTERN_Gfx(ttc_seg7_dl_0700B1D8);
EXTERN_Gfx(ttc_seg7_dl_0700E878);
EXTERN_Gfx(ttc_seg7_dl_0700ECB8);
EXTERN_Gfx(ttc_seg7_dl_0700EFE0);
EXTERN_Gfx(ttc_seg7_dl_0700F760);
EXTERN_Gfx(ttc_seg7_dl_0700FBB8);
EXTERN_Gfx(ttc_seg7_dl_0700FFE8);
EXTERN_Gfx(ttc_seg7_dl_070102B8);
EXTERN_Gfx(ttc_seg7_dl_07010868);
EXTERN_Gfx(ttc_seg7_dl_07010D38);
EXTERN_Gfx(ttc_seg7_dl_07011040);
EXTERN_Gfx(ttc_seg7_dl_07011360);
EXTERN_Gfx(ttc_seg7_dl_070116A8);
EXTERN_Gfx(ttc_seg7_dl_07011B38);
EXTERN_Gfx(ttc_seg7_dl_07012028);
EXTERN_Gfx(ttc_seg7_dl_07012148);
EXTERN_Gfx(ttc_seg7_dl_07012278);
EXTERN_Collision(ttc_seg7_collision_level);
EXTERN_Collision(ttc_seg7_collision_07014F70);
EXTERN_Collision(ttc_seg7_collision_07015008);
EXTERN_Collision(ttc_seg7_collision_clock_pendulum);
EXTERN_Collision(ttc_seg7_collision_070152B4);
EXTERN_Collision(ttc_seg7_collision_070153E0);
EXTERN_Collision(ttc_seg7_collision_sliding_surface);
EXTERN_Collision(ttc_seg7_collision_07015584);
EXTERN_Collision(ttc_seg7_collision_07015650);
EXTERN_Collision(ttc_seg7_collision_07015754);
EXTERN_Collision(ttc_seg7_collision_070157D8);
EXTERN_Collision(ttc_seg7_collision_clock_platform);
EXTERN_Collision(ttc_seg7_collision_clock_main_rotation);
EXTERN_Collision(ttc_seg7_collision_rotating_clock_platform2);
EXTERN_MacroObject(ttc_seg7_macro_objs);
EXTERN_u8(ttc_yellow_triangle);
EXTERN_Gfx(ttc_dl_surface_treadmill_begin);
EXTERN_Gfx(ttc_dl_surface_treadmill_end);
extern Movtex ttc_movtex_tris_big_surface_treadmill[];
extern Movtex ttc_movtex_tris_small_surface_treadmill[];
EXTERN_Gfx(ttc_dl_surface_treadmill);

// script
EXTERN_LevelScript(level_ttc_entry);

#endif
