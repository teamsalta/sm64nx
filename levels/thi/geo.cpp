#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/thi/header.h"

#include "levels/thi/areas/1/6/geo.inc.c"
#include "levels/thi/areas/1/7/geo.inc.c"
#include "levels/thi/areas/1/geo.inc.c"
#include "levels/thi/areas/2/geo.inc.c"
#include "levels/thi/areas/3/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(thi_geo_0005B0);
	EXPORT_GEO(thi_geo_0005C8);
	EXPORT_GEO(thi_geo_0005F0);
	EXPORT_GEO(thi_level_geo_000608);
	EXPORT_GEO(thi_level_geo_0006D4);
	EXPORT_GEO(thi_level_geo_00079C);
}
