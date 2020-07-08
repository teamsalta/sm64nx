#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/rr/header.h"

#include "levels/rr/areas/1/2/geo.inc.c"
#include "levels/rr/areas/1/3/geo.inc.c"
#include "levels/rr/areas/1/4/geo.inc.c"
#include "levels/rr/areas/1/5/geo.inc.c"
#include "levels/rr/areas/1/6/geo.inc.c"
#include "levels/rr/areas/1/7/geo.inc.c"
#include "levels/rr/areas/1/8/geo.inc.c"
#include "levels/rr/areas/1/9/geo.inc.c"
#include "levels/rr/areas/1/10/geo.inc.c"
#include "levels/rr/areas/1/11/geo.inc.c"
#include "levels/rr/areas/1/12/geo.inc.c"
#include "levels/rr/areas/1/13/geo.inc.c"
#include "levels/rr/areas/1/14/geo.inc.c"
#include "levels/rr/areas/1/15/geo.inc.c"
#include "levels/rr/areas/1/16/geo.inc.c"
#include "levels/rr/areas/1/17/geo.inc.c"
#include "levels/rr/areas/1/18/geo.inc.c"
#include "levels/rr/areas/1/19/geo.inc.c"
#include "levels/rr/areas/1/20/geo.inc.c"
#include "levels/rr/areas/1/21/geo.inc.c"
#include "levels/rr/flying_carpet/geo.inc.c"
#include "levels/rr/swinging_platform/geo.inc.c"
#include "levels/rr/rotating_bridge_platform/geo.inc.c"
#include "levels/rr/cruiser_wing/geo.inc.c"
#include "levels/rr/octagonal_platform/geo.inc.c"
#include "levels/rr/sliding_platform/geo.inc.c"
#include "levels/rr/pyramid_platform/geo.inc.c" // unused
#include "levels/rr/elevator_platform/geo.inc.c"
#include "levels/rr/seesaw_platform/geo.inc.c"
#include "levels/rr/donut_block/geo.inc.c"
#include "levels/rr/l_platform/geo.inc.c" // also unused
#include "levels/rr/tricky_triangles_1/geo.inc.c"
#include "levels/rr/tricky_triangles_2/geo.inc.c"
#include "levels/rr/tricky_triangles_3/geo.inc.c"
#include "levels/rr/tricky_triangles_4/geo.inc.c"
#include "levels/rr/tricky_triangles_5/geo.inc.c"
#include "levels/rr/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(rr_geo_000660);
	EXPORT_GEO(rr_geo_000678);
	EXPORT_GEO(rr_geo_000690);
	EXPORT_GEO(rr_geo_0006A8);
	EXPORT_GEO(rr_geo_0006C0);
	EXPORT_GEO(rr_geo_0006D8);
	EXPORT_GEO(rr_geo_0006F0);
	EXPORT_GEO(rr_geo_000708);
	EXPORT_GEO(rr_geo_000720);
	EXPORT_GEO(rr_geo_000738);
	EXPORT_GEO(rr_geo_000758);
	EXPORT_GEO(rr_geo_000770);
	EXPORT_GEO(rr_geo_000788);
	EXPORT_GEO(rr_geo_0007A0);
	EXPORT_GEO(rr_geo_0007B8);
	EXPORT_GEO(rr_geo_0007D0);
	EXPORT_GEO(rr_geo_0007E8);
	EXPORT_GEO(rr_geo_000800);
	EXPORT_GEO(rr_geo_000818);
	EXPORT_GEO(rr_geo_000830);
	EXPORT_GEO(rr_geo_000848);
	EXPORT_GEO(rr_geo_000860);
	EXPORT_GEO(rr_geo_000878);
	EXPORT_GEO(rr_geo_000890);
	EXPORT_GEO(rr_geo_0008A8);
	EXPORT_GEO(rr_geo_0008C0);
	EXPORT_GEO(rr_geo_0008F0);
	EXPORT_GEO(rr_geo_000908);
	EXPORT_GEO(rr_geo_000920);
	EXPORT_GEO(rr_geo_000958);
	EXPORT_GEO(rr_geo_000970);
	EXPORT_GEO(rr_geo_000988);
	EXPORT_GEO(rr_geo_0009A0);
	EXPORT_GEO(rr_geo_0009B8);
	EXPORT_GEO(rr_level_geo);
}
