#ifndef JRB_HEADER_H
#define JRB_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(jrb_geo_000900);
EXTERN_GeoLayout(jrb_geo_000918);
EXTERN_GeoLayout(jrb_geo_000930);
EXTERN_GeoLayout(jrb_geo_000948);
EXTERN_GeoLayout(jrb_geo_000960);
EXTERN_GeoLayout(jrb_geo_000978);
EXTERN_GeoLayout(jrb_geo_000990);
EXTERN_GeoLayout(jrb_geo_0009B0);
EXTERN_GeoLayout(jrb_geo_0009C8);
EXTERN_GeoLayout(jrb_geo_0009E8);
EXTERN_GeoLayout(jrb_geo_000A00);
EXTERN_GeoLayout(jrb_level_geo);
EXTERN_GeoLayout(jrb_geo_000AFC);

// leveldata
EXTERN_Gfx(jrb_seg7_dl_07002FD0);
EXTERN_Gfx(jrb_seg7_dl_07004940);
EXTERN_Gfx(jrb_seg7_dl_07004C78);
EXTERN_Gfx(jrb_seg7_dl_070058C8);
EXTERN_Gfx(jrb_seg7_dl_070069B0);
EXTERN_Gfx(jrb_seg7_dl_07007570);
EXTERN_Gfx(jrb_seg7_dl_07007718);
EXTERN_Gfx(jrb_seg7_dl_07007AC8);
EXTERN_Gfx(jrb_seg7_dl_07007DC8);
EXTERN_Gfx(jrb_seg7_dl_070080F8);
EXTERN_Gfx(jrb_seg7_dl_07008FD8);
EXTERN_Gfx(jrb_seg7_dl_070090B0);
EXTERN_Gfx(jrb_seg7_dl_07009A58);
EXTERN_Gfx(jrb_seg7_dl_07009B30);
EXTERN_Gfx(jrb_seg7_dl_0700A608);
EXTERN_Gfx(jrb_seg7_dl_0700AC68);
EXTERN_Gfx(jrb_seg7_dl_0700AE48);
EXTERN_Gfx(jrb_seg7_dl_0700AFB0);
EXTERN_Collision(jrb_seg7_area_1_collision);
EXTERN_MacroObject(jrb_seg7_area_1_macro_objs);
EXTERN_Collision(jrb_seg7_collision_rock_solid);
EXTERN_Collision(jrb_seg7_collision_floating_platform);
EXTERN_Collision(jrb_seg7_collision_floating_box);
EXTERN_Collision(jrb_seg7_collision_in_sunken_ship_3);
EXTERN_Collision(jrb_seg7_collision_in_sunken_ship);
EXTERN_Collision(jrb_seg7_collision_0700CEF0);
EXTERN_Collision(jrb_seg7_collision_in_sunken_ship_2);
EXTERN_Collision(jrb_seg7_collision_pillar_base);
EXTERN_Collision(jrb_seg7_collision_0700D1DC);
EXTERN_Trajectory(jrb_seg7_trajectory_unagi_1);
EXTERN_Trajectory(jrb_seg7_trajectory_unagi_2);
EXTERN_MovtexQuadCollection(jrb_movtex_water);
EXTERN_MovtexQuadCollection(jrb_movtex_intial_mist);
EXTERN_Gfx(jrb_seg7_dl_0700EF00);
EXTERN_Gfx(jrb_seg7_dl_0700FE48);
EXTERN_Gfx(jrb_seg7_dl_07010548);
EXTERN_Collision(jrb_seg7_area_2_collision);
EXTERN_MacroObject(jrb_seg7_area_2_macro_objs);
EXTERN_MovtexQuadCollection(jrb_movtex_sinked_boat_water);

// script
EXTERN_LevelScript(level_jrb_entry);

#endif
