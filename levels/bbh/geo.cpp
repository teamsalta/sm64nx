#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/bbh/header.h"

#include "levels/bbh/staircase_step/geo.inc.c"
#include "levels/bbh/tilting_trap_platform/geo.inc.c"
#include "levels/bbh/tumbling_platform_far/geo.inc.c"
#include "levels/bbh/tumbling_platform_near/geo.inc.c"
#include "levels/bbh/moving_bookshelf/geo.inc.c"
#include "levels/bbh/mesh_elevator/geo.inc.c"
#include "levels/bbh/merry_go_round/geo.inc.c"
#include "levels/bbh/coffin/geo.inc.c"
#include "levels/bbh/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(geo_bbh_0005B0);
	EXPORT_GEO(geo_bbh_0005C8);
	EXPORT_GEO(geo_bbh_0005E0);
	EXPORT_GEO(geo_bbh_0005F8);
	EXPORT_GEO(geo_bbh_000610);
	EXPORT_GEO(geo_bbh_000628);
	EXPORT_GEO(geo_bbh_000640);
	EXPORT_GEO(geo_bbh_000658);
	EXPORT_GEO(geo_bbh_000670);
	EXPORT_GEO(geo_bbh_0006B0);
	EXPORT_GEO(geo_bbh_0006E8);
	EXPORT_GEO(geo_bbh_000730);
	EXPORT_GEO(geo_bbh_000750);
	EXPORT_GEO(geo_bbh_000768);
	EXPORT_GEO(geo_bbh_0007B0);
	EXPORT_GEO(geo_bbh_0007D0);
	EXPORT_GEO(geo_bbh_000800);
	EXPORT_GEO(geo_bbh_000828);
	EXPORT_GEO(geo_bbh_000860);
	EXPORT_GEO(geo_bbh_000888);
	EXPORT_GEO(geo_bbh_0008B0);
	EXPORT_GEO(geo_bbh_0008E8);
	EXPORT_GEO(geo_bbh_000950);
	EXPORT_GEO(geo_bbh_0009C8);
	EXPORT_GEO(geo_bbh_000A18);
	EXPORT_GEO(geo_bbh_000A60);
	EXPORT_GEO(geo_bbh_000AD8);
	EXPORT_GEO(geo_bbh_000B28);
	EXPORT_GEO(geo_bbh_000B88);
	EXPORT_GEO(geo_bbh_000BF0);
	EXPORT_GEO(geo_bbh_000C38);
	EXPORT_GEO(geo_bbh_000C88);
	EXPORT_GEO(geo_bbh_000CE8);
	EXPORT_GEO(geo_bbh_000D20);
	EXPORT_GEO(geo_bbh_000D68);
	EXPORT_GEO(geo_bbh_000DB0);
	EXPORT_GEO(geo_bbh_000DF0);
	EXPORT_GEO(geo_bbh_000E40);
	EXPORT_GEO(geo_bbh_000E80);
	EXPORT_GEO(geo_bbh_000EB0);
	EXPORT_GEO(bbh_level_geo);
}
