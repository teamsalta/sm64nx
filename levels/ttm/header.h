#ifndef TTM_HEADER_H
#define TTM_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(ttm_geo_000710);
EXTERN_GeoLayout(ttm_geo_000730);
EXTERN_GeoLayout(ttm_geo_000748);
EXTERN_GeoLayout(ttm_geo_000778);
EXTERN_GeoLayout(ttm_geo_0007A8);
EXTERN_GeoLayout(ttm_geo_0007D8);
EXTERN_GeoLayout(ttm_geo_000808);
EXTERN_GeoLayout(ttm_geo_000830);
EXTERN_GeoLayout(ttm_geo_000858);
EXTERN_GeoLayout(ttm_geo_000880);
EXTERN_GeoLayout(ttm_geo_0008A8);
EXTERN_GeoLayout(ttm_geo_0008D0);
EXTERN_GeoLayout(ttm_geo_0008F8);
EXTERN_GeoLayout(ttm_geo_000920);
EXTERN_GeoLayout(ttm_geo_000948);
EXTERN_GeoLayout(ttm_geo_000970);
EXTERN_GeoLayout(ttm_geo_000990);
EXTERN_GeoLayout(ttm_geo_0009C0);
EXTERN_GeoLayout(ttm_geo_0009F0);
EXTERN_GeoLayout(ttm_geo_000A18);
EXTERN_GeoLayout(ttm_geo_000A40);
EXTERN_GeoLayout(ttm_level_geo);
EXTERN_GeoLayout(ttm_slide_level_geo_000B5C);
EXTERN_GeoLayout(ttm_slide_level_geo_000BEC);
EXTERN_GeoLayout(ttm_slide_level_geo_000C84);
EXTERN_GeoLayout(ttm_geo_000D14);
EXTERN_GeoLayout(ttm_geo_000D4C);
EXTERN_GeoLayout(ttm_geo_000D84);
EXTERN_GeoLayout(ttm_geo_000DBC);
EXTERN_GeoLayout(ttm_geo_000DF4);

// leveldata
EXTERN_Gfx(ttm_seg7_dl_0700A120);
EXTERN_Gfx(ttm_seg7_dl_0700A2E0);
EXTERN_Gfx(ttm_seg7_dl_0700A8C0);
EXTERN_Gfx(ttm_seg7_dl_0700AB08);
EXTERN_Gfx(ttm_seg7_dl_0700AF90);
EXTERN_Gfx(ttm_seg7_dl_0700B0D0);
EXTERN_Gfx(ttm_seg7_dl_0700B530);
EXTERN_Gfx(ttm_seg7_dl_0700B670);
EXTERN_Gfx(ttm_seg7_dl_0700BAD0);
EXTERN_Gfx(ttm_seg7_dl_0700BC10);
EXTERN_Gfx(ttm_seg7_dl_0700BE20);
EXTERN_Gfx(ttm_seg7_dl_0700C070);
EXTERN_Gfx(ttm_seg7_dl_0700C408);
EXTERN_Gfx(ttm_seg7_dl_0700CAE0);
EXTERN_Gfx(ttm_seg7_dl_0700CD10);
EXTERN_Gfx(ttm_seg7_dl_0700D1D8);
EXTERN_Gfx(ttm_seg7_dl_0700D688);
EXTERN_Gfx(ttm_seg7_dl_0700DF78);
EXTERN_Gfx(ttm_seg7_dl_0700E308);
EXTERN_Gfx(ttm_seg7_dl_0700EAC8);
EXTERN_Gfx(ttm_seg7_dl_0700EC58);
EXTERN_Gfx(ttm_seg7_dl_0700F270);
EXTERN_Gfx(ttm_seg7_dl_0700FA18);
EXTERN_Gfx(ttm_seg7_dl_07010978);
EXTERN_Gfx(ttm_seg7_dl_07010A68);
EXTERN_Gfx(ttm_seg7_dl_07011128);
EXTERN_Gfx(ttm_seg7_dl_07011608);
EXTERN_Gfx(ttm_seg7_dl_07011C78);
EXTERN_Gfx(ttm_seg7_dl_07011D78);
EXTERN_Gfx(ttm_seg7_dl_07012270);
EXTERN_Gfx(ttm_seg7_dl_07012388);
EXTERN_Gfx(ttm_seg7_dl_070123A0);
EXTERN_Gfx(ttm_seg7_dl_070123B8);
EXTERN_Gfx(ttm_seg7_dl_07012410);
extern struct Painting ttm_slide_painting;
EXTERN_Gfx(ttm_seg7_dl_07013430);
EXTERN_Gfx(ttm_seg7_dl_07013608);
EXTERN_Collision(ttm_seg7_collision_pitoune_2);
EXTERN_Collision(ttm_seg7_collision_ukiki_cage);
EXTERN_Collision(ttm_seg7_area_1_collision);
EXTERN_MacroObject(ttm_seg7_area_1_macro_objs);
EXTERN_Trajectory(ttm_seg7_trajectory_070170A0);
EXTERN_MovtexQuadCollection(ttm_movtex_puddle);
extern Movtex ttm_movtex_tris_begin_waterfall[];
extern Movtex ttm_movtex_tris_begin_puddle_waterfall[];
extern Movtex ttm_movtex_tris_end_waterfall[];
extern Movtex ttm_movtex_tris_end_puddle_waterfall[];
extern Movtex ttm_movtex_tris_puddle_waterfall[];
EXTERN_Gfx(ttm_dl_waterfall);
EXTERN_Gfx(ttm_dl_bottom_waterfall);
EXTERN_Gfx(ttm_dl_puddle_waterfall);
EXTERN_Gfx(ttm_seg7_dl_0701D798);
EXTERN_Gfx(ttm_seg7_dl_0701DBB8);
EXTERN_Gfx(ttm_seg7_dl_070249A0);
EXTERN_Gfx(ttm_seg7_dl_07024C78);
EXTERN_Gfx(ttm_seg7_dl_07025F48);
EXTERN_Gfx(ttm_seg7_dl_0702A1B8);
EXTERN_Gfx(ttm_seg7_dl_0702A8A0);
EXTERN_Gfx(ttm_seg7_dl_0702A9C0);
EXTERN_Gfx(ttm_seg7_dl_0702AAA8);
EXTERN_Gfx(ttm_seg7_dl_0702AB90);
EXTERN_Gfx(ttm_seg7_dl_0702AC78);
EXTERN_Gfx(ttm_seg7_dl_0702BB60);
EXTERN_Collision(ttm_seg7_area_2_collision);
EXTERN_Collision(ttm_seg7_area_3_collision);
EXTERN_Collision(ttm_seg7_area_4_collision);
EXTERN_Collision(ttm_seg7_collision_podium_warp);
EXTERN_MacroObject(ttm_seg7_area_2_macro_objs);
EXTERN_MacroObject(ttm_seg7_area_3_macro_objs);
EXTERN_MacroObject(ttm_seg7_area_4_macro_objs);

// script
EXTERN_LevelScript(level_ttm_entry);

#endif
