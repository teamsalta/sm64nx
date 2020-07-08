#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/ddd/header.h"

#include "levels/ddd/pole/geo.inc.c"
#include "levels/ddd/sub_door/geo.inc.c"
#include "levels/ddd/submarine/geo.inc.c"
#include "levels/ddd/areas/1/geo.inc.c"
#include "levels/ddd/areas/2/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(ddd_geo_000450);
	EXPORT_GEO(ddd_geo_000478);
	EXPORT_GEO(ddd_geo_0004A0);
	EXPORT_GEO(ddd_geo_0004C0);
	EXPORT_GEO(ddd_level_geo);
}
