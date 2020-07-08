#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group3.h"

#include "king_bobomb/geo.inc.c"
#include "water_bubble/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(king_bobomb_geo);
	EXPORT_GEO(water_bomb_geo);
	EXPORT_GEO(water_bomb_shadow_geo);
}
