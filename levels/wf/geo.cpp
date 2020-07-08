#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/wf/header.h"

#include "levels/wf/areas/1/5/geo.inc.c"
#include "levels/wf/areas/1/6/geo.inc.c"
#include "levels/wf/areas/1/7/geo.inc.c"
#include "levels/wf/areas/1/8/geo.inc.c"
#include "levels/wf/areas/1/9/geo.inc.c"
#include "levels/wf/areas/1/10/geo.inc.c"
#include "levels/wf/areas/1/11/geo.inc.c"
#include "levels/wf/areas/1/12/geo.inc.c"
#include "levels/wf/areas/1/13/geo.inc.c"
#include "levels/wf/areas/1/15/geo.inc.c"
#include "levels/wf/areas/1/16/geo.inc.c"
#include "levels/wf/areas/1/17/geo.inc.c"
#include "levels/wf/areas/1/18/geo.inc.c"
#include "levels/wf/areas/1/19/geo.inc.c"
#include "levels/wf/small_bomp/geo.inc.c"
#include "levels/wf/large_bomp/geo.inc.c"
#include "levels/wf/rotating_wooden_platform/geo.inc.c"
#include "levels/wf/sliding_platform/geo.inc.c"
#include "levels/wf/tumbling_bridge_near/geo.inc.c"
#include "levels/wf/tumbling_bridge_far/geo.inc.c"
#include "levels/wf/areas/1/14/geo.inc.c"
#include "levels/wf/beta_extending_platform/geo.inc.c"
#include "levels/wf/extending_platform/geo.inc.c"
#include "levels/wf/breakable_wall_right/geo.inc.c"
#include "levels/wf/breakable_wall_left/geo.inc.c"
#include "levels/wf/kickable_board/geo.inc.c"
#include "levels/wf/tower_door/geo.inc.c"
#include "levels/wf/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(wf_geo_0007E0);
	EXPORT_GEO(wf_geo_000820);
	EXPORT_GEO(wf_geo_000860);
	EXPORT_GEO(wf_geo_000878);
	EXPORT_GEO(wf_geo_000890);
	EXPORT_GEO(wf_geo_0008A8);
	EXPORT_GEO(wf_geo_0008E8);
	EXPORT_GEO(wf_geo_000900);
	EXPORT_GEO(wf_geo_000940);
	EXPORT_GEO(wf_geo_000958);
	EXPORT_GEO(wf_geo_0009A0);
	EXPORT_GEO(wf_geo_0009B8);
	EXPORT_GEO(wf_geo_0009D0);
	EXPORT_GEO(wf_geo_0009E8);
	EXPORT_GEO(wf_geo_000A00);
	EXPORT_GEO(wf_geo_000A40);
	EXPORT_GEO(wf_geo_000A58);
	EXPORT_GEO(wf_geo_000A98);
	EXPORT_GEO(wf_geo_000AB0);
	EXPORT_GEO(wf_geo_000AC8);
	EXPORT_GEO(wf_geo_000AE0);
	EXPORT_GEO(wf_geo_000AF8);
	EXPORT_GEO(wf_geo_000B10);
	EXPORT_GEO(wf_geo_000B38);
	EXPORT_GEO(wf_geo_000B60);
	EXPORT_GEO(wf_geo_000B78);
	EXPORT_GEO(wf_geo_000B90);
	EXPORT_GEO(wf_geo_000BA8);
	EXPORT_GEO(wf_geo_000BC8);
	EXPORT_GEO(wf_geo_000BE0);
	EXPORT_GEO(wf_level_geo);
}
