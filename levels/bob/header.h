#ifndef BOB_HEADER_H
#define BOB_HEADER_H

#include "types.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(bob_geo_000440);
EXTERN_GeoLayout(bob_geo_000458);
EXTERN_GeoLayout(bob_geo_000470);
EXTERN_GeoLayout(bob_level_geo);

// leveldata
EXTERN_Gfx(bob_seg7_dl_07004390);
EXTERN_Gfx(bob_seg7_dl_07009D80);
EXTERN_Gfx(bob_seg7_dl_0700A470);
EXTERN_Gfx(bob_seg7_dl_0700A920);
EXTERN_Gfx(bob_seg7_dl_0700DD18);
EXTERN_Gfx(bob_seg7_dl_0700E338);
EXTERN_Gfx(bob_seg7_dl_0700E458);
EXTERN_Gfx(bob_seg7_dl_0700E768);
EXTERN_Gfx(bob_seg7_dl_0700E8A0);
EXTERN_Collision(bob_seg7_collision_level);
EXTERN_MacroObject(bob_seg7_macro_objs);
EXTERN_Collision(bob_seg7_collision_chain_chomp_gate);
EXTERN_Collision(bob_seg7_collision_bridge);
EXTERN_Collision(bob_seg7_collision_gate);
EXTERN_Trajectory(bob_seg7_metal_ball_path0);
EXTERN_Trajectory(bob_seg7_metal_ball_path1);
EXTERN_Trajectory(bob_seg7_trajectory_koopa);

// script
EXTERN_LevelScript(level_bob_entry);

#endif
