#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/lll/header.h"

#include "levels/lll/areas/1/4/geo.inc.c"
#include "levels/lll/areas/1/5/geo.inc.c"
#include "levels/lll/areas/1/6/geo.inc.c"
#include "levels/lll/areas/1/7/geo.inc.c"
#include "levels/lll/areas/1/8/geo.inc.c"
#include "levels/lll/areas/1/9/geo.inc.c"
#include "levels/lll/areas/1/10/geo.inc.c"
#include "levels/lll/areas/1/11/geo.inc.c"
#include "levels/lll/areas/1/12/geo.inc.c"
#include "levels/lll/areas/1/13/geo.inc.c"
#include "levels/lll/areas/1/14/geo.inc.c"
#include "levels/lll/areas/1/15/geo.inc.c"
#include "levels/lll/moving_octagonal_mesh_platform/geo.inc.c"
#include "levels/lll/drawbridge_part/geo.inc.c"
#include "levels/lll/rotating_block_fire_bars/geo.inc.c"
#include "levels/lll/wooden_float_small/geo.inc.c"
#include "levels/lll/wooden_float_large/geo.inc.c"
#include "levels/lll/collapsing_wooden_platform/geo.inc.c"
#include "levels/lll/long_wooden_bridge/geo.inc.c"
#include "levels/lll/rotating_hexagonal_ring/geo.inc.c"
#include "levels/lll/sinking_rectangular_platform/geo.inc.c"
#include "levels/lll/sinking_square_platform/geo.inc.c"
#include "levels/lll/tilting_square_platform/geo.inc.c"
#include "levels/lll/puzzle_piece/geo.inc.c"
#include "levels/lll/sinking_rock_block/geo.inc.c"
#include "levels/lll/rolling_log/geo.inc.c"
#include "levels/lll/areas/1/geo.inc.c"
#include "levels/lll/volcano_falling_trap/geo.inc.c"
#include "levels/lll/areas/2/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(lll_geo_0009E0);
	EXPORT_GEO(lll_geo_0009F8);
	EXPORT_GEO(lll_geo_000A10);
	EXPORT_GEO(lll_geo_000A28);
	EXPORT_GEO(lll_geo_000A40);
	EXPORT_GEO(lll_geo_000A60);
	EXPORT_GEO(lll_geo_000A78);
	EXPORT_GEO(lll_geo_000A90);
	EXPORT_GEO(lll_geo_000AA8);
	EXPORT_GEO(lll_geo_000AC0);
	EXPORT_GEO(lll_geo_000AD8);
	EXPORT_GEO(lll_geo_000AF0);
	EXPORT_GEO(lll_geo_000B08);
	EXPORT_GEO(lll_geo_000B20);
	EXPORT_GEO(lll_geo_000B38);
	EXPORT_GEO(lll_geo_000B50);
	EXPORT_GEO(lll_geo_000B68);
	EXPORT_GEO(lll_geo_000B80);
	EXPORT_GEO(lll_geo_000B98);
	EXPORT_GEO(lll_geo_000BB0);
	EXPORT_GEO(lll_geo_000BC8);
	EXPORT_GEO(lll_geo_000BE0);
	EXPORT_GEO(lll_geo_000BF8);
	EXPORT_GEO(lll_geo_000C10);
	EXPORT_GEO(lll_geo_000C30);
	EXPORT_GEO(lll_geo_000C50);
	EXPORT_GEO(lll_geo_000C70);
	EXPORT_GEO(lll_geo_000C90);
	EXPORT_GEO(lll_geo_000CB0);
	EXPORT_GEO(lll_geo_000CD0);
	EXPORT_GEO(lll_geo_000CF0);
	EXPORT_GEO(lll_geo_000D10);
	EXPORT_GEO(lll_geo_000D30);
	EXPORT_GEO(lll_geo_000D50);
	EXPORT_GEO(lll_geo_000D70);
	EXPORT_GEO(lll_geo_000D90);
	EXPORT_GEO(lll_geo_000DB0);
	EXPORT_GEO(lll_geo_000DD0);
	EXPORT_GEO(lll_geo_000DE8);
	EXPORT_GEO(lll_level_geo);
	EXPORT_GEO(lll_geo_000EA8);
	EXPORT_GEO(lll_volcano_level_geo);
}
