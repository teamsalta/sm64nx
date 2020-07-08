#ifndef HMC_HEADER_H
#define HMC_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(hmc_geo_000530);
EXTERN_GeoLayout(hmc_geo_000548);
EXTERN_GeoLayout(hmc_geo_000570);
EXTERN_GeoLayout(hmc_geo_000588);
EXTERN_GeoLayout(hmc_geo_0005A0);
EXTERN_GeoLayout(hmc_geo_0005B8);
EXTERN_GeoLayout(hmc_geo_0005D0);
EXTERN_GeoLayout(hmc_geo_0005E8);
EXTERN_GeoLayout(hmc_geo_000618);
EXTERN_GeoLayout(hmc_geo_000658);
EXTERN_GeoLayout(hmc_geo_0006A8);
EXTERN_GeoLayout(hmc_geo_0006E0);
EXTERN_GeoLayout(hmc_geo_000700);
EXTERN_GeoLayout(hmc_geo_000748);
EXTERN_GeoLayout(hmc_geo_000770);
EXTERN_GeoLayout(hmc_geo_000798);
EXTERN_GeoLayout(hmc_geo_0007F8);
EXTERN_GeoLayout(hmc_geo_000850);
EXTERN_GeoLayout(hmc_geo_0008D0);
EXTERN_GeoLayout(hmc_geo_000938);
EXTERN_GeoLayout(hmc_geo_000998);
EXTERN_GeoLayout(hmc_geo_000A18);
EXTERN_GeoLayout(hmc_geo_000A88);
EXTERN_GeoLayout(hmc_geo_000AE8);
EXTERN_GeoLayout(hmc_geo_000B48);
EXTERN_GeoLayout(hmc_level_geo);

// leveldata
EXTERN_Gfx(hmc_seg7_dl_070078B0);
EXTERN_Gfx(hmc_seg7_dl_07007B50);
EXTERN_Gfx(hmc_seg7_dl_070080E8);
EXTERN_Gfx(hmc_seg7_dl_070093F0);
EXTERN_Gfx(hmc_seg7_dl_0700E448);
EXTERN_Gfx(hmc_seg7_dl_0700EF00);
EXTERN_Gfx(hmc_seg7_dl_0700F3E8);
EXTERN_Gfx(hmc_seg7_dl_0700FA40);
EXTERN_Gfx(hmc_seg7_dl_0700FEF0);
EXTERN_Gfx(hmc_seg7_dl_07010070);
EXTERN_Gfx(hmc_seg7_dl_07013CA8);
EXTERN_Gfx(hmc_seg7_dl_07013E80);
EXTERN_Gfx(hmc_seg7_dl_07014300);
EXTERN_Gfx(hmc_seg7_dl_07014B08);
EXTERN_Gfx(hmc_seg7_dl_07014C00);
EXTERN_Gfx(hmc_seg7_dl_07014E48);
EXTERN_Gfx(hmc_seg7_dl_070173A8);
EXTERN_Gfx(hmc_seg7_dl_07017C98);
EXTERN_Gfx(hmc_seg7_dl_07018200);
EXTERN_Gfx(hmc_seg7_dl_07019248);
EXTERN_Gfx(hmc_seg7_dl_07019368);
EXTERN_Gfx(hmc_seg7_dl_0701A080);
EXTERN_Gfx(hmc_seg7_dl_0701A400);
EXTERN_Gfx(hmc_seg7_dl_0701E820);
EXTERN_Gfx(hmc_seg7_dl_0701F1B0);
EXTERN_Gfx(hmc_seg7_dl_0701F690);
EXTERN_Gfx(hmc_seg7_dl_0701F818);
EXTERN_Gfx(hmc_seg7_dl_0701FD58);
EXTERN_Gfx(hmc_seg7_dl_0701FFF8);
EXTERN_Gfx(hmc_seg7_dl_07020FD0);
EXTERN_Gfx(hmc_seg7_dl_07021760);
EXTERN_Gfx(hmc_seg7_dl_07021BA0);
EXTERN_Gfx(hmc_seg7_dl_070228A0);
EXTERN_Gfx(hmc_seg7_dl_07022AA0);
EXTERN_Gfx(hmc_seg7_dl_07022DA0);
EXTERN_Gfx(hmc_seg7_dl_07023090);
EXTERN_Gfx(hmc_seg7_dl_07023BC8);
EXTERN_Gfx(hmc_seg7_dl_07023E10);
EXTERN_Gfx(hmc_seg7_dl_07024110);
EXTERN_Gfx(hmc_seg7_dl_07024268);
EXTERN_Gfx(hmc_seg7_dl_070242A0);
EXTERN_u8(*const hmc_seg7_painting_textures_07025518);
extern struct Painting cotmc_painting;
EXTERN_Collision(hmc_seg7_collision_level);
EXTERN_MacroObject(hmc_seg7_macro_objs);
EXTERN_u8(hmc_seg7_rooms);
EXTERN_Collision(hmc_seg7_collision_elevator);
EXTERN_Collision(hmc_seg7_collision_0702B65C);
EXTERN_Collision(hmc_seg7_collision_controllable_platform);
EXTERN_Collision(hmc_seg7_collision_controllable_platform_sub);
EXTERN_Trajectory(hmc_seg7_trajectory_0702B86C);
EXTERN_MovtexQuadCollection(hmc_movtex_dorrie_pool_water);
EXTERN_MovtexQuadCollection(hmc_movtex_toxic_maze_mist);

// script
EXTERN_LevelScript(level_hmc_entry);

#endif
