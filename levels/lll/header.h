#ifndef LLL_HEADER_H
#define LLL_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(lll_geo_0009E0);
EXTERN_GeoLayout(lll_geo_0009F8);
EXTERN_GeoLayout(lll_geo_000A10);
EXTERN_GeoLayout(lll_geo_000A28);
EXTERN_GeoLayout(lll_geo_000A40);
EXTERN_GeoLayout(lll_geo_000A60);
EXTERN_GeoLayout(lll_geo_000A78);
EXTERN_GeoLayout(lll_geo_000A90);
EXTERN_GeoLayout(lll_geo_000AA8);
EXTERN_GeoLayout(lll_geo_000AC0);
EXTERN_GeoLayout(lll_geo_000AD8);
EXTERN_GeoLayout(lll_geo_000AF0);
EXTERN_GeoLayout(lll_geo_000B08);
EXTERN_GeoLayout(lll_geo_000B20);
EXTERN_GeoLayout(lll_geo_000B38);
EXTERN_GeoLayout(lll_geo_000B50);
EXTERN_GeoLayout(lll_geo_000B68);
EXTERN_GeoLayout(lll_geo_000B80);
EXTERN_GeoLayout(lll_geo_000B98);
EXTERN_GeoLayout(lll_geo_000BB0);
EXTERN_GeoLayout(lll_geo_000BC8);
EXTERN_GeoLayout(lll_geo_000BE0);
EXTERN_GeoLayout(lll_geo_000BF8);
EXTERN_GeoLayout(lll_geo_000C10);
EXTERN_GeoLayout(lll_geo_000C30);
EXTERN_GeoLayout(lll_geo_000C50);
EXTERN_GeoLayout(lll_geo_000C70);
EXTERN_GeoLayout(lll_geo_000C90);
EXTERN_GeoLayout(lll_geo_000CB0);
EXTERN_GeoLayout(lll_geo_000CD0);
EXTERN_GeoLayout(lll_geo_000CF0);
EXTERN_GeoLayout(lll_geo_000D10);
EXTERN_GeoLayout(lll_geo_000D30);
EXTERN_GeoLayout(lll_geo_000D50);
EXTERN_GeoLayout(lll_geo_000D70);
EXTERN_GeoLayout(lll_geo_000D90);
EXTERN_GeoLayout(lll_geo_000DB0);
EXTERN_GeoLayout(lll_geo_000DD0);
EXTERN_GeoLayout(lll_geo_000DE8);
EXTERN_GeoLayout(lll_level_geo);
EXTERN_GeoLayout(lll_geo_000EA8);
EXTERN_GeoLayout(lll_volcano_level_geo);

// leveldata
EXTERN_Gfx(lll_seg7_dl_070134E0);
EXTERN_Gfx(lll_seg7_dl_070137C0);
EXTERN_Gfx(lll_seg7_dl_070138F8);
EXTERN_Gfx(lll_seg7_dl_07013D28);
EXTERN_Gfx(lll_seg7_dl_07014788);
EXTERN_Gfx(lll_seg7_dl_07014BD8);
EXTERN_Gfx(lll_seg7_dl_07015458);
EXTERN_Gfx(lll_seg7_dl_07015C88);
EXTERN_Gfx(lll_seg7_dl_07015E20);
EXTERN_Gfx(lll_seg7_dl_07016250);
EXTERN_Gfx(lll_seg7_dl_070165C8);
EXTERN_Gfx(lll_seg7_dl_07016B00);
EXTERN_Gfx(lll_seg7_dl_070174E0);
EXTERN_Gfx(lll_seg7_dl_070178A8);
EXTERN_Gfx(lll_seg7_dl_07017B50);
EXTERN_Gfx(lll_seg7_dl_07017F40);
EXTERN_Gfx(lll_seg7_dl_07018380);
EXTERN_Gfx(lll_seg7_dl_07018680);
EXTERN_Gfx(lll_seg7_dl_07018A30);
EXTERN_Gfx(lll_seg7_dl_07018C90);
EXTERN_Gfx(lll_seg7_dl_07018EF8);
EXTERN_Gfx(lll_seg7_dl_07019160);
EXTERN_Gfx(lll_seg7_dl_070193E0);
EXTERN_Gfx(lll_seg7_dl_07019A08);
EXTERN_Gfx(lll_seg7_dl_07019C08);
EXTERN_Gfx(lll_seg7_dl_0701A010);
EXTERN_Gfx(lll_seg7_dl_0701A1F0);
EXTERN_Gfx(lll_seg7_dl_0701A388);
EXTERN_Gfx(lll_seg7_dl_0701A3B8);
EXTERN_Gfx(lll_seg7_dl_0701A3E8);
EXTERN_Gfx(lll_seg7_dl_0701A418);
EXTERN_Gfx(lll_seg7_dl_0701A448);
EXTERN_Gfx(lll_seg7_dl_0701A478);
EXTERN_Gfx(lll_seg7_dl_0701A4A8);
EXTERN_Gfx(lll_seg7_dl_0701A4D8);
EXTERN_Gfx(lll_seg7_dl_0701A508);
EXTERN_Gfx(lll_seg7_dl_0701A538);
EXTERN_Gfx(lll_seg7_dl_0701A568);
EXTERN_Gfx(lll_seg7_dl_0701A598);
EXTERN_Gfx(lll_seg7_dl_0701A5C8);
EXTERN_Gfx(lll_seg7_dl_0701A5F8);
EXTERN_Gfx(lll_seg7_dl_0701A628);
EXTERN_Gfx(lll_seg7_dl_0701A878);
EXTERN_Gfx(lll_seg7_dl_0701AD70);
EXTERN_Collision(lll_seg7_area_1_collision);
EXTERN_MacroObject(lll_seg7_area_1_macro_objs);
EXTERN_Collision(lll_seg7_collision_octagonal_moving_platform);
EXTERN_Collision(lll_seg7_collision_drawbridge);
EXTERN_Collision(lll_seg7_collision_rotating_fire_bars);
EXTERN_Collision(lll_seg7_collision_wood_piece);
EXTERN_Collision(lll_seg7_collision_0701D21C);
EXTERN_Collision(lll_seg7_collision_rotating_platform);
EXTERN_Collision(lll_seg7_collision_slow_tilting_platform);
EXTERN_Collision(lll_seg7_collision_sinking_pyramids);
EXTERN_Collision(lll_seg7_collision_inverted_pyramid);
EXTERN_Collision(lll_seg7_collision_puzzle_piece);
EXTERN_Collision(lll_seg7_collision_floating_block);
EXTERN_Collision(lll_seg7_collision_pitoune);
EXTERN_Collision(lll_seg7_collision_hexagonal_platform);
EXTERN_Gfx(lll_seg7_dl_070235C8);
EXTERN_Gfx(lll_seg7_dl_07024C18);
EXTERN_Gfx(lll_seg7_dl_070255D8);
EXTERN_Gfx(lll_seg7_dl_07025A48);
EXTERN_Gfx(lll_seg7_dl_07025BD8);
EXTERN_Gfx(lll_seg7_dl_07025EC0);
EXTERN_Collision(lll_seg7_area_2_collision);
EXTERN_MacroObject(lll_seg7_area_2_macro_objs);
EXTERN_Collision(lll_seg7_collision_falling_wall);
EXTERN_Trajectory(lll_seg7_trajectory_0702856C);
EXTERN_Trajectory(lll_seg7_trajectory_07028660);
extern Movtex lll_movtex_tris_lava_floor[];
EXTERN_Gfx(lll_dl_lava_floor);
EXTERN_MovtexQuadCollection(lll_movtex_volcano_floor_lava);
extern Movtex lll_movtex_tris_lavafall_volcano[];
EXTERN_Gfx(lll_dl_lavafall_volcano);

// script
EXTERN_LevelScript(level_lll_entry);

#endif
