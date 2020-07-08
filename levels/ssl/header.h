#ifndef SSL_HEADER_H
#define SSL_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(ssl_geo_0005C0);
EXTERN_GeoLayout(ssl_geo_0005D8);
EXTERN_GeoLayout(ssl_geo_000618);
EXTERN_GeoLayout(ssl_geo_000630);
EXTERN_GeoLayout(ssl_level_geo);
EXTERN_GeoLayout(ssl_geo_000734);
EXTERN_GeoLayout(ssl_geo_000764);
EXTERN_GeoLayout(ssl_geo_000794);
EXTERN_GeoLayout(ssl_geo_0007AC);
EXTERN_GeoLayout(ssl_pyramid_level_geo);
EXTERN_GeoLayout(ssl_geo_00088C);

// leveldata
EXTERN_u8(ssl_pyramid_sand);
EXTERN_u8(ssl_quicksand);
EXTERN_Gfx(ssl_dl_quicksand_pit_begin);
EXTERN_Gfx(ssl_dl_quicksand_pit_end);
EXTERN_Gfx(ssl_dl_pyramid_quicksand_pit_begin);
EXTERN_Gfx(ssl_dl_pyramid_quicksand_pit_end);
extern Movtex ssl_movtex_tris_quicksand_pit[];
extern Movtex ssl_movtex_tris_pyramid_quicksand_pit[];
EXTERN_Gfx(ssl_dl_quicksand_pit);
EXTERN_Gfx(ssl_dl_pyramid_quicksand_pit_static);
EXTERN_Gfx(ssl_seg7_dl_07009F48);
EXTERN_Gfx(ssl_seg7_dl_0700BA78);
EXTERN_Gfx(ssl_seg7_dl_0700BC18);
EXTERN_Gfx(ssl_seg7_dl_0700BD00);
EXTERN_Gfx(ssl_seg7_dl_0700BF18);
EXTERN_Gfx(ssl_seg7_dl_0700FCE0);
EXTERN_Collision(ssl_seg7_area_1_collision);
EXTERN_MacroObject(ssl_seg7_area_1_macro_objs);
EXTERN_Collision(ssl_seg7_collision_pyramid_top);
EXTERN_Collision(ssl_seg7_collision_tox_box);
EXTERN_MovtexQuadCollection(ssl_movtex_puddle_water);
EXTERN_MovtexQuadCollection(ssl_movtex_toxbox_quicksand_mist);
EXTERN_Gfx(ssl_dl_quicksand_begin);
EXTERN_Gfx(ssl_dl_quicksand_end);
extern Movtex ssl_movtex_tris_pyramid_quicksand[];
EXTERN_Gfx(ssl_dl_pyramid_quicksand);
extern Movtex ssl_movtex_tris_pyramid_corners_quicksand[];
EXTERN_Gfx(ssl_dl_pyramid_corners_quicksand);
extern Movtex ssl_movtex_tris_sides_quicksand[];
EXTERN_Gfx(ssl_dl_sides_quicksand);
EXTERN_Gfx(ssl_seg7_dl_0701EE80);
EXTERN_Gfx(ssl_seg7_dl_0701F920);
EXTERN_Gfx(ssl_seg7_dl_0701FCE0);
EXTERN_Gfx(ssl_seg7_dl_07021A08);
EXTERN_Gfx(ssl_seg7_dl_07021DE8);
EXTERN_Gfx(ssl_seg7_dl_070220A8);
EXTERN_Gfx(ssl_seg7_dl_070221E8);
EXTERN_Gfx(ssl_seg7_dl_070228A8);
EXTERN_Gfx(ssl_seg7_dl_070229E8);
EXTERN_Gfx(ssl_seg7_dl_07022CF8);
EXTERN_Gfx(ssl_seg7_dl_070233A8);
EXTERN_Gfx(ssl_seg7_dl_070235C0);
EXTERN_Collision(ssl_seg7_area_2_collision);
EXTERN_Collision(ssl_seg7_area_3_collision);
EXTERN_MacroObject(ssl_seg7_area_2_macro_objs);
EXTERN_MacroObject(ssl_seg7_area_3_macro_objs);
EXTERN_Collision(ssl_seg7_collision_grindel);
EXTERN_Collision(ssl_seg7_collision_spindel);
EXTERN_Collision(ssl_seg7_collision_0702808C);
EXTERN_Collision(ssl_seg7_collision_pyramid_elevator);
EXTERN_Collision(ssl_seg7_collision_07028274);
EXTERN_Collision(ssl_seg7_collision_070282F8);
EXTERN_Collision(ssl_seg7_collision_07028370);
EXTERN_Collision(ssl_seg7_collision_070284B0);
EXTERN_Gfx(ssl_dl_pyramid_sand_pathway_floor_begin);
EXTERN_Gfx(ssl_dl_pyramid_sand_pathway_floor_end);
EXTERN_Gfx(ssl_dl_pyramid_sand_pathway_begin);
EXTERN_Gfx(ssl_dl_pyramid_sand_pathway_end);
extern Movtex ssl_movtex_tris_pyramid_sand_pathway_front[];
EXTERN_Gfx(ssl_dl_pyramid_sand_pathway_front_end);
extern Movtex ssl_movtex_tris_pyramid_sand_pathway_floor[];
extern Movtex ssl_movtex_tris_pyramid_sand_pathway_side[];
EXTERN_Gfx(ssl_dl_pyramid_sand_pathway_side_end);

// script
EXTERN_LevelScript(level_ssl_entry);

#endif
