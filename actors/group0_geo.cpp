#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group0.h"

#include "bubble/geo.inc.c"
#include "walk_smoke/geo.inc.c"
#include "burn_smoke/geo.inc.c"
#include "stomp_smoke/geo.inc.c"
#include "water_waves/geo.inc.c"
#include "sparkle/geo.inc.c"
#include "water_splash/geo.inc.c"
#include "sparkle_animation/geo.inc.c"
#include "mario/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(mario_geo);
	EXPORT_GEO(bubble_geo);
	EXPORT_GEO(purple_marble_geo);
	EXPORT_GEO(smoke_geo);
	EXPORT_GEO(burn_smoke_geo);
	EXPORT_GEO(spot_on_ground_geo);
	EXPORT_GEO(mario_TODO_geo_0000E0);
	EXPORT_GEO(water_waves_surface_geo);
	EXPORT_GEO(water_waves_geo);
	EXPORT_GEO(sparkles_geo);
	EXPORT_GEO(water_splash_geo);
	EXPORT_GEO(sparkles_animation_geo);
}
