#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/jrb/header.h"

#include "levels/jrb/falling_pillar/geo.inc.c"
#include "levels/jrb/falling_pillar_base/geo.inc.c"
#include "levels/jrb/rock/geo.inc.c"
#include "levels/jrb/floating_platform/geo.inc.c"
#include "levels/jrb/sliding_box/geo.inc.c"
#include "levels/jrb/wooden_ship/geo.inc.c"
#include "levels/jrb/areas/1/geo.inc.c"
#include "levels/jrb/areas/2/geo.inc.c"


#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(jrb_geo_000900);
	EXPORT_GEO(jrb_geo_000918);
	EXPORT_GEO(jrb_geo_000930);
	EXPORT_GEO(jrb_geo_000948);
	EXPORT_GEO(jrb_geo_000960);
	EXPORT_GEO(jrb_geo_000978);
	EXPORT_GEO(jrb_geo_000990);
	EXPORT_GEO(jrb_geo_0009B0);
	EXPORT_GEO(jrb_geo_0009C8);
	EXPORT_GEO(jrb_geo_0009E8);
	EXPORT_GEO(jrb_geo_000A00);
	EXPORT_GEO(jrb_level_geo);
	EXPORT_GEO(jrb_geo_000AFC);
}
