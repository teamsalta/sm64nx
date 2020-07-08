#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/bowser_3/header.h"

#include "levels/bowser_3/falling_platform_1/geo.inc.c"
#include "levels/bowser_3/falling_platform_2/geo.inc.c"
#include "levels/bowser_3/falling_platform_3/geo.inc.c"
#include "levels/bowser_3/falling_platform_4/geo.inc.c"
#include "levels/bowser_3/falling_platform_5/geo.inc.c"
#include "levels/bowser_3/falling_platform_6/geo.inc.c"
#include "levels/bowser_3/falling_platform_7/geo.inc.c"
#include "levels/bowser_3/falling_platform_8/geo.inc.c"
#include "levels/bowser_3/falling_platform_9/geo.inc.c"
#include "levels/bowser_3/falling_platform_10/geo.inc.c"
#include "levels/bowser_3/areas/1/bomb_stand/geo.inc.c"
#include "levels/bowser_3/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(bowser_3_geo_000290);
	EXPORT_GEO(bowser_3_geo_0002A8);
	EXPORT_GEO(bowser_3_geo_0002C0);
	EXPORT_GEO(bowser_3_geo_0002D8);
	EXPORT_GEO(bowser_3_geo_0002F0);
	EXPORT_GEO(bowser_3_geo_000308);
	EXPORT_GEO(bowser_3_geo_000320);
	EXPORT_GEO(bowser_3_geo_000338);
	EXPORT_GEO(bowser_3_geo_000350);
	EXPORT_GEO(bowser_3_geo_000368);
	EXPORT_GEO(bowser_3_geo_000380);
	EXPORT_GEO(bowser_3_level_geo);
}
