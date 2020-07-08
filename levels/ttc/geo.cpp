#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/ttc/header.h"

#include "levels/ttc/rotating_cube/geo.inc.c"
#include "levels/ttc/rotating_prism/geo.inc.c"
#include "levels/ttc/pendulum/geo.inc.c"
#include "levels/ttc/large_treadmill/geo.inc.c"
#include "levels/ttc/small_treadmill/geo.inc.c"
#include "levels/ttc/push_block/geo.inc.c"
#include "levels/ttc/rotating_hexagon/geo.inc.c"
#include "levels/ttc/rotating_triangle/geo.inc.c"
#include "levels/ttc/pit_block/geo.inc.c"
#include "levels/ttc/pit_block_2/geo.inc.c"
#include "levels/ttc/elevator_platform/geo.inc.c"
#include "levels/ttc/clock_hand/geo.inc.c"
#include "levels/ttc/spinner/geo.inc.c"
#include "levels/ttc/small_gear/geo.inc.c"
#include "levels/ttc/large_gear/geo.inc.c"
#include "levels/ttc/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(ttc_geo_000240);
	EXPORT_GEO(ttc_geo_000258);
	EXPORT_GEO(ttc_geo_000270);
	EXPORT_GEO(ttc_geo_000288);
	EXPORT_GEO(ttc_geo_0002A8);
	EXPORT_GEO(ttc_geo_0002C8);
	EXPORT_GEO(ttc_geo_0002E0);
	EXPORT_GEO(ttc_geo_0002F8);
	EXPORT_GEO(ttc_geo_000310);
	EXPORT_GEO(ttc_geo_000328);
	EXPORT_GEO(ttc_geo_000340);
	EXPORT_GEO(ttc_geo_000358);
	EXPORT_GEO(ttc_geo_000370);
	EXPORT_GEO(ttc_geo_000388);
	EXPORT_GEO(ttc_geo_0003A0);
	EXPORT_GEO(ttc_level_geo);
}
