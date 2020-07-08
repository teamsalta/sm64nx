#ifndef CASTLE_COURTYARD_HEADER_H
#define CASTLE_COURTYARD_HEADER_H

#include "types.h"
#include "game/moving_texture.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(castle_courtyard_geo_000200);
EXTERN_GeoLayout(castle_courtyard_level_geo);

// leveldata
EXTERN_Gfx(castle_courtyard_seg7_dl_070048B8);
EXTERN_Gfx(castle_courtyard_seg7_dl_07005078);
EXTERN_Gfx(castle_courtyard_seg7_dl_07005698);
EXTERN_Gfx(castle_courtyard_seg7_dl_07005938);
EXTERN_Collision(castle_courtyard_seg7_collision);
EXTERN_MacroObject(castle_courtyard_seg7_macro_objs);
EXTERN_MovtexQuadCollection(castle_courtyard_movtex_star_statue_water);

// script
EXTERN_LevelScript(level_castle_courtyard_entry);

#endif
