#ifndef COTMC_HEADER_H
#define COTMC_HEADER_H

#include "types.h"
#include "game/debug.h"

// geo
EXTERN_GeoLayout(cotmc_level_geo);

// leveldata
EXTERN_Gfx(cotmc_seg7_dl_07007D48);
EXTERN_Gfx(cotmc_seg7_dl_0700A160);
EXTERN_Gfx(cotmc_seg7_dl_0700A4B8);
EXTERN_Collision(cotmc_seg7_collision_level);
EXTERN_MacroObject(cotmc_seg7_macro_objs);
EXTERN_Gfx(cotmc_dl_water_begin);
EXTERN_Gfx(cotmc_dl_water_end);
extern Movtex cotmc_movtex_tris_water[];
EXTERN_Gfx(cotmc_dl_water);

// script
EXTERN_LevelScript(level_cotmc_entry);

#endif
