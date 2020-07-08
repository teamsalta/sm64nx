#ifndef BOWSER_2_HEADER_H
#define BOWSER_2_HEADER_H

#include "types.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(bowser_2_geo_000170);
EXTERN_GeoLayout(bowser_2_level_geo);

// leveldata
EXTERN_Gfx(bowser_2_seg7_dl_07000D30);
EXTERN_Gfx(bowser_2_seg7_dl_07000FE0);
EXTERN_Gfx(bowser_2_seg7_dl_07001930);
EXTERN_Collision(bowser_2_seg7_collision_lava);
EXTERN_Collision(bowser_2_seg7_collision_tilting_platform);

// script
EXTERN_LevelScript(level_bowser_2_entry);

#endif
