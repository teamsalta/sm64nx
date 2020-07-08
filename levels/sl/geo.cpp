#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/sl/header.h"

#include "levels/sl/unused_cracked_ice/geo.inc.c"
#include "levels/sl/unused_ice_shard/geo.inc.c"
#include "levels/sl/snow_mound/geo.inc.c"
#include "levels/sl/areas/1/geo.inc.c"
#include "levels/sl/areas/2/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(sl_geo_000360);
	EXPORT_GEO(sl_geo_000378);
	EXPORT_GEO(sl_geo_000390);
	EXPORT_GEO(sl_level_geo);
	EXPORT_GEO(sl_underground_level_geo);
}
