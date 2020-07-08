#ifndef BBH_HEADER_H
#define BBH_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(geo_bbh_0005B0);
EXTERN_GeoLayout(geo_bbh_0005C8);
EXTERN_GeoLayout(geo_bbh_0005E0);
EXTERN_GeoLayout(geo_bbh_0005F8);
EXTERN_GeoLayout(geo_bbh_000610);
EXTERN_GeoLayout(geo_bbh_000628);
EXTERN_GeoLayout(geo_bbh_000640);
EXTERN_GeoLayout(geo_bbh_000658);
EXTERN_GeoLayout(geo_bbh_000670);
EXTERN_GeoLayout(geo_bbh_0006B0);
EXTERN_GeoLayout(geo_bbh_0006E8);
EXTERN_GeoLayout(geo_bbh_000730);
EXTERN_GeoLayout(geo_bbh_000750);
EXTERN_GeoLayout(geo_bbh_000768);
EXTERN_GeoLayout(geo_bbh_0007B0);
EXTERN_GeoLayout(geo_bbh_0007D0);
EXTERN_GeoLayout(geo_bbh_000800);
EXTERN_GeoLayout(geo_bbh_000828);
EXTERN_GeoLayout(geo_bbh_000860);
EXTERN_GeoLayout(geo_bbh_000888);
EXTERN_GeoLayout(geo_bbh_0008B0);
EXTERN_GeoLayout(geo_bbh_0008E8);
EXTERN_GeoLayout(geo_bbh_000950);
EXTERN_GeoLayout(geo_bbh_0009C8);
EXTERN_GeoLayout(geo_bbh_000A18);
EXTERN_GeoLayout(geo_bbh_000A60);
EXTERN_GeoLayout(geo_bbh_000AD8);
EXTERN_GeoLayout(geo_bbh_000B28);
EXTERN_GeoLayout(geo_bbh_000B88);
EXTERN_GeoLayout(geo_bbh_000BF0);
EXTERN_GeoLayout(geo_bbh_000C38);
EXTERN_GeoLayout(geo_bbh_000C88);
EXTERN_GeoLayout(geo_bbh_000CE8);
EXTERN_GeoLayout(geo_bbh_000D20);
EXTERN_GeoLayout(geo_bbh_000D68);
EXTERN_GeoLayout(geo_bbh_000DB0);
EXTERN_GeoLayout(geo_bbh_000DF0);
EXTERN_GeoLayout(geo_bbh_000E40);
EXTERN_GeoLayout(geo_bbh_000E80);
EXTERN_GeoLayout(geo_bbh_000EB0);
EXTERN_GeoLayout(bbh_level_geo);

// leveldata
EXTERN_Gfx(bbh_seg7_dl_070075A8);
EXTERN_Gfx(bbh_seg7_dl_07007940);
EXTERN_Gfx(bbh_seg7_dl_07007B90);
EXTERN_Gfx(bbh_seg7_dl_07007FD0);
EXTERN_Gfx(bbh_seg7_dl_07008B58);
EXTERN_Gfx(bbh_seg7_dl_07008EA8);
EXTERN_Gfx(bbh_seg7_dl_0700AFF0);
EXTERN_Gfx(bbh_seg7_dl_0700B1C8);
EXTERN_Gfx(bbh_seg7_dl_0700B418);
EXTERN_Gfx(bbh_seg7_dl_0700B9E0);
EXTERN_Gfx(bbh_seg7_dl_0700BBF8);
EXTERN_Gfx(bbh_seg7_dl_0700D080);
EXTERN_Gfx(bbh_seg7_dl_0700D2E0);
EXTERN_Gfx(bbh_seg7_dl_0700D490);
EXTERN_Gfx(bbh_seg7_dl_0700D7E0);
EXTERN_Gfx(bbh_seg7_dl_0700F510);
EXTERN_Gfx(bbh_seg7_dl_0700F848);
EXTERN_Gfx(bbh_seg7_dl_07011120);
EXTERN_Gfx(bbh_seg7_dl_07012220);
EXTERN_Gfx(bbh_seg7_dl_07012510);
EXTERN_Gfx(bbh_seg7_dl_070126E8);
EXTERN_Gfx(bbh_seg7_dl_070139A8);
EXTERN_Gfx(bbh_seg7_dl_07013BE8);
EXTERN_Gfx(bbh_seg7_dl_07014FD8);
EXTERN_Gfx(bbh_seg7_dl_07015398);
EXTERN_Gfx(bbh_seg7_dl_070156E0);
EXTERN_Gfx(bbh_seg7_dl_07015A20);
EXTERN_Gfx(bbh_seg7_dl_07017378);
EXTERN_Gfx(bbh_seg7_dl_07017570);
EXTERN_Gfx(bbh_seg7_dl_07017788);
EXTERN_Gfx(bbh_seg7_dl_07019EF8);
EXTERN_Gfx(bbh_seg7_dl_0701A080);
EXTERN_Gfx(bbh_seg7_dl_0701A850);
EXTERN_Gfx(bbh_seg7_dl_0701B6D0);
EXTERN_Gfx(bbh_seg7_dl_0701E4E0);
EXTERN_Gfx(bbh_seg7_dl_0701E8D8);
EXTERN_Gfx(bbh_seg7_dl_0701ED18);
EXTERN_Gfx(bbh_seg7_dl_0701EEC8);
EXTERN_Gfx(bbh_seg7_dl_0701F070);
EXTERN_Gfx(bbh_seg7_dl_0701F2E8);
EXTERN_Gfx(bbh_seg7_dl_0701F5F8);
EXTERN_Gfx(bbh_seg7_dl_0701F7E8);
EXTERN_Gfx(bbh_seg7_dl_0701FAB0);
EXTERN_Gfx(bbh_seg7_dl_0701FD28);
EXTERN_Gfx(bbh_seg7_dl_0701FFE8);
EXTERN_Gfx(bbh_seg7_dl_070202F0);
EXTERN_Gfx(bbh_seg7_dl_070206F0);
EXTERN_Collision(bbh_seg7_collision_level);
EXTERN_u8(bbh_seg7_rooms);
EXTERN_MacroObject(bbh_seg7_macro_objs);
EXTERN_Collision(bbh_seg7_collision_staircase_step);
EXTERN_Collision(bbh_seg7_collision_tilt_floor_platform);
EXTERN_Collision(bbh_seg7_collision_07026B1C);
EXTERN_Collision(bbh_seg7_collision_haunted_bookshelf);
EXTERN_Collision(bbh_seg7_collision_mesh_elevator);
EXTERN_Collision(bbh_seg7_collision_merry_go_round);
EXTERN_Collision(bbh_seg7_collision_coffin);
EXTERN_MovtexQuadCollection(bbh_movtex_merry_go_round_water_entrance);
EXTERN_MovtexQuadCollection(bbh_movtex_merry_go_round_water_side);

// script
EXTERN_LevelScript(level_bbh_entry);

#endif
