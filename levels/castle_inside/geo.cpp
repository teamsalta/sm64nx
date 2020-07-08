#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/castle_inside/header.h"

#include "levels/castle_inside/star_door/geo.inc.c"
#include "levels/castle_inside/trap_door/geo.inc.c"
#include "levels/castle_inside/areas/1/geo.inc.c"
#include "levels/castle_inside/areas/2/pendulum/geo.inc.c" // Another weird case
#include "levels/castle_inside/clock_minute_hand/geo.inc.c"
#include "levels/castle_inside/clock_hour_hand/geo.inc.c"
#include "levels/castle_inside/areas/2/geo.inc.c"
#include "levels/castle_inside/water_level_pillar/geo.inc.c"
#include "levels/castle_inside/areas/3/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(castle_geo_000F00);
	EXPORT_GEO(castle_geo_000F18);
	EXPORT_GEO(castle_geo_000F30);
	EXPORT_GEO(castle_geo_000F70);
	EXPORT_GEO(castle_geo_000F88);
	EXPORT_GEO(castle_geo_000FA8);
	EXPORT_GEO(castle_geo_000FD0);
	EXPORT_GEO(castle_geo_001000);
	EXPORT_GEO(castle_geo_001038);
	EXPORT_GEO(castle_geo_001088);
	EXPORT_GEO(castle_geo_0010C8);
	EXPORT_GEO(castle_geo_001110);
	EXPORT_GEO(castle_geo_001158);
	EXPORT_GEO(castle_geo_0011A8);
	EXPORT_GEO(castle_geo_001200);
	EXPORT_GEO(castle_geo_001260);
	EXPORT_GEO(castle_geo_0012C8);
	EXPORT_GEO(castle_geo_001348);
	EXPORT_GEO(castle_geo_0013B8);
	EXPORT_GEO(castle_geo_001400);
	EXPORT_GEO(castle_geo_001530);
	EXPORT_GEO(castle_geo_001548);
	EXPORT_GEO(castle_geo_001560);
	EXPORT_GEO(castle_geo_001578);
	EXPORT_GEO(castle_geo_0015B8);
	EXPORT_GEO(castle_geo_0015F8);
	EXPORT_GEO(castle_geo_001628);
	EXPORT_GEO(castle_geo_001668);
	EXPORT_GEO(castle_geo_001690);
	EXPORT_GEO(castle_geo_0016D8);
	EXPORT_GEO(castle_geo_001740);
	EXPORT_GEO(castle_geo_001798);
	EXPORT_GEO(castle_geo_001800);
	EXPORT_GEO(castle_geo_001858);
	EXPORT_GEO(castle_geo_001940);
	EXPORT_GEO(castle_geo_001958);
	EXPORT_GEO(castle_geo_001980);
	EXPORT_GEO(castle_geo_0019C8);
	EXPORT_GEO(castle_geo_0019F8);
	EXPORT_GEO(castle_geo_001A30);
	EXPORT_GEO(castle_geo_001A58);
	EXPORT_GEO(castle_geo_001AB8);
	EXPORT_GEO(castle_geo_001AF8);
	EXPORT_GEO(castle_geo_001B48);
	EXPORT_GEO(castle_geo_001BB0);
	EXPORT_GEO(castle_inside_level_geo);
}
