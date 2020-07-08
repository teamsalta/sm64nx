#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/ccm/header.h"

#include "levels/ccm/ropeway_lift/geo.inc.c"
#include "levels/ccm/snowman_base/geo.inc.c"
#include "levels/ccm/snowman_head/geo.inc.c"
#include "levels/ccm/areas/1/6/geo.inc.c"
#include "levels/ccm/areas/1/7/geo.inc.c"
#include "levels/ccm/areas/1/8/geo.inc.c"
#include "levels/ccm/areas/1/9/geo.inc.c"
#include "levels/ccm/areas/1/10/geo.inc.c"
#include "levels/ccm/areas/1/geo.inc.c"
#include "levels/ccm/areas/2/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(ccm_geo_0003D0);
	EXPORT_GEO(ccm_geo_0003F0);
	EXPORT_GEO(ccm_geo_00040C);
	EXPORT_GEO(ccm_geo_00042C);
	EXPORT_GEO(ccm_geo_00045C);
	EXPORT_GEO(ccm_geo_000494);
	EXPORT_GEO(ccm_geo_0004BC);
	EXPORT_GEO(ccm_geo_0004E4);
	EXPORT_GEO(ccm_level_geo);
	EXPORT_GEO(ccm_geo_0005E8);
}
