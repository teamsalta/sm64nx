#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/bitfs/header.h"

#include "levels/bitfs/areas/1/2/geo.inc.c"
#include "levels/bitfs/areas/1/3/geo.inc.c"
#include "levels/bitfs/areas/1/4/geo.inc.c"
#include "levels/bitfs/areas/1/5/geo.inc.c"
#include "levels/bitfs/areas/1/6/geo.inc.c"
#include "levels/bitfs/areas/1/7/geo.inc.c"
#include "levels/bitfs/areas/1/8/geo.inc.c"
#include "levels/bitfs/areas/1/9/geo.inc.c"
#include "levels/bitfs/areas/1/10/geo.inc.c"
#include "levels/bitfs/areas/1/11/geo.inc.c"
#include "levels/bitfs/areas/1/12/geo.inc.c"
#include "levels/bitfs/areas/1/13/geo.inc.c"
#include "levels/bitfs/areas/1/14/geo.inc.c"
#include "levels/bitfs/areas/1/15/geo.inc.c"
#include "levels/bitfs/areas/1/16/geo.inc.c"
#include "levels/bitfs/areas/1/17/geo.inc.c"
#include "levels/bitfs/areas/1/18/geo.inc.c"
#include "levels/bitfs/areas/1/19/geo.inc.c"
#include "levels/bitfs/areas/1/20/geo.inc.c"
#include "levels/bitfs/elevator/geo.inc.c"
#include "levels/bitfs/sinking_cage_platform/geo.inc.c"
#include "levels/bitfs/sinking_cage_pole/geo.inc.c"
#include "levels/bitfs/tilting_square_platform/geo.inc.c"
#include "levels/bitfs/tumbling_platform_near/geo.inc.c"
#include "levels/bitfs/tumbling_platform_far/geo.inc.c"
#include "levels/bitfs/stretching_platform/geo.inc.c"
#include "levels/bitfs/moving_square_platform/geo.inc.c"
#include "levels/bitfs/sliding_platform/geo.inc.c"
#include "levels/bitfs/platform_on_track/geo.inc.c"
#include "levels/bitfs/sinking_platforms/geo.inc.c"
#include "levels/bitfs/seesaw_platform/geo.inc.c"
#include "levels/bitfs/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(bitfs_geo_0004B0);
	EXPORT_GEO(bitfs_geo_0004C8);
	EXPORT_GEO(bitfs_geo_0004E0);
	EXPORT_GEO(bitfs_geo_0004F8);
	EXPORT_GEO(bitfs_geo_000510);
	EXPORT_GEO(bitfs_geo_000528);
	EXPORT_GEO(bitfs_geo_000540);
	EXPORT_GEO(bitfs_geo_000558);
	EXPORT_GEO(bitfs_geo_000570);
	EXPORT_GEO(bitfs_geo_000588);
	EXPORT_GEO(bitfs_geo_0005A0);
	EXPORT_GEO(bitfs_geo_0005B8);
	EXPORT_GEO(bitfs_geo_0005D0);
	EXPORT_GEO(bitfs_geo_0005E8);
	EXPORT_GEO(bitfs_geo_000600);
	EXPORT_GEO(bitfs_geo_000618);
	EXPORT_GEO(bitfs_geo_000630);
	EXPORT_GEO(bitfs_geo_000648);
	EXPORT_GEO(bitfs_geo_000660);
	EXPORT_GEO(bitfs_geo_000678);
	EXPORT_GEO(bitfs_geo_000690);
	EXPORT_GEO(bitfs_geo_0006A8);
	EXPORT_GEO(bitfs_geo_0006C0);
	EXPORT_GEO(bitfs_geo_0006D8);
	EXPORT_GEO(bitfs_geo_0006F0);
	EXPORT_GEO(bitfs_geo_000708);
	EXPORT_GEO(bitfs_geo_000728);
	EXPORT_GEO(bitfs_geo_000740);
	EXPORT_GEO(bitfs_geo_000758);
	EXPORT_GEO(bitfs_geo_000770);
	EXPORT_GEO(bitfs_geo_000788);
	EXPORT_GEO(bitfs_level_geo);
}
