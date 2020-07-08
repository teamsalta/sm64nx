#ifndef INTRO_HEADER_H
#define INTRO_HEADER_H

#include "types.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(intro_geo_0002D0);
EXTERN_GeoLayout(intro_geo_00035C);
EXTERN_GeoLayout(intro_geo_0003B8);
EXTERN_GeoLayout(intro_level_geo);

// leveldata
#include "game/segment7.h"

// script
EXTERN_LevelScript(level_intro_entry_1);
EXTERN_LevelScript(level_intro_entry_2);
EXTERN_LevelScript(level_intro_entry_3);
EXTERN_LevelScript(level_intro_entry_4);
EXTERN_LevelScript(script_intro_L1);
EXTERN_LevelScript(script_intro_L2);
EXTERN_LevelScript(script_intro_L3);
EXTERN_LevelScript(script_intro_L4);
EXTERN_LevelScript(script_intro_L5);

#endif
