#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/bitdw/header.h"

#include "levels/bitdw/areas/1/starting_platform/geo.inc.c"
#include "levels/bitdw/areas/1/large_platform/geo.inc.c"
#include "levels/bitdw/areas/1/wooden_bridge_and_fences/geo.inc.c"
#include "levels/bitdw/areas/1/quartzy_path_1/geo.inc.c"
#include "levels/bitdw/areas/1/quartzy_path_2/geo.inc.c"
#include "levels/bitdw/areas/1/quartzy_path_fences/geo.inc.c"
#include "levels/bitdw/areas/1/octogonal_platform/geo.inc.c"
#include "levels/bitdw/areas/1/platform_with_hill/geo.inc.c"
#include "levels/bitdw/areas/1/wooden_platform/geo.inc.c"
#include "levels/bitdw/areas/1/platforms_and_tilting/geo.inc.c"
#include "levels/bitdw/areas/1/platforms_and_tilting_2/geo.inc.c"
#include "levels/bitdw/areas/1/quartz_crystal/geo.inc.c"
#include "levels/bitdw/areas/1/staircase_slope_and_platform/geo.inc.c"
#include "levels/bitdw/areas/1/track_for_pyramid_platforms/geo.inc.c"
#include "levels/bitdw/areas/1/narrow_path_platform/geo.inc.c"
#include "levels/bitdw/sliding_platform/geo.inc.c"
#include "levels/bitdw/seesaw_platform/geo.inc.c"
#include "levels/bitdw/square_platform/geo.inc.c"
#include "levels/bitdw/ferris_wheel_axle/geo.inc.c"
#include "levels/bitdw/ferris_platform/geo.inc.c"
#include "levels/bitdw/collapsing_stairs_1/geo.inc.c"
#include "levels/bitdw/collapsing_stairs_2/geo.inc.c"
#include "levels/bitdw/collapsing_stairs_3/geo.inc.c"
#include "levels/bitdw/collapsing_stairs_4/geo.inc.c"
#include "levels/bitdw/collapsing_stairs_5/geo.inc.c"
#include "levels/bitdw/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(geo_bitdw_0003C0);
	EXPORT_GEO(geo_bitdw_0003D8);
	EXPORT_GEO(geo_bitdw_0003F0);
	EXPORT_GEO(geo_bitdw_000408);
	EXPORT_GEO(geo_bitdw_000420);
	EXPORT_GEO(geo_bitdw_000438);
	EXPORT_GEO(geo_bitdw_000450);
	EXPORT_GEO(geo_bitdw_000468);
	EXPORT_GEO(geo_bitdw_000480);
	EXPORT_GEO(geo_bitdw_000498);
	EXPORT_GEO(geo_bitdw_0004B0);
	EXPORT_GEO(geo_bitdw_0004C8);
	EXPORT_GEO(geo_bitdw_0004E0);
	EXPORT_GEO(geo_bitdw_0004F8);
	EXPORT_GEO(geo_bitdw_000510);
	EXPORT_GEO(geo_bitdw_000528);
	EXPORT_GEO(geo_bitdw_000540);
	EXPORT_GEO(geo_bitdw_000558);
	EXPORT_GEO(geo_bitdw_000570);
	EXPORT_GEO(geo_bitdw_000588);
	EXPORT_GEO(geo_bitdw_0005A0);
	EXPORT_GEO(geo_bitdw_0005B8);
	EXPORT_GEO(geo_bitdw_0005D0);
	EXPORT_GEO(geo_bitdw_0005E8);
	EXPORT_GEO(geo_bitdw_000600);
	EXPORT_GEO(bitdw_level_geo);
}
