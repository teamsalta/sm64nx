#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/castle_grounds/header.h"

#include "levels/castle_grounds/areas/1/11/geo.inc.c"
#include "levels/castle_grounds/areas/1/3/geo.inc.c"
#include "levels/castle_grounds/areas/1/7/geo.inc.c"
#include "levels/castle_grounds/areas/1/8/geo.inc.c"
#include "levels/castle_grounds/areas/1/geo.inc.c"


#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(castle_grounds_geo_000660);
	EXPORT_GEO(castle_grounds_geo_0006F4);
	EXPORT_GEO(castle_grounds_geo_00070C);
	EXPORT_GEO(castle_grounds_geo_000724);
	EXPORT_GEO(castle_grounds_level_geo);
}

