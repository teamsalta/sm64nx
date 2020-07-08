#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/wdw/header.h"

#include "levels/wdw/square_floating_platform/geo.inc.c"
#include "levels/wdw/arrow_lift/geo.inc.c"
#include "levels/wdw/water_level_diamond/geo.inc.c"
#include "levels/wdw/hidden_platform/geo.inc.c"
#include "levels/wdw/express_elevator/geo.inc.c"
#include "levels/wdw/rectangular_floating_platform/geo.inc.c"
#include "levels/wdw/rotating_platform/geo.inc.c"
#include "levels/wdw/areas/1/geo.inc.c"
#include "levels/wdw/areas/2/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(wdw_geo_000580);
	EXPORT_GEO(wdw_geo_000598);
	EXPORT_GEO(wdw_geo_0005C0);
	EXPORT_GEO(wdw_geo_0005E8);
	EXPORT_GEO(wdw_geo_000610);
	EXPORT_GEO(wdw_geo_000628);
	EXPORT_GEO(wdw_geo_000640);
	EXPORT_GEO(wdw_level_geo);
	EXPORT_GEO(wdw_town_level_geo);
}
