#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/ssl/header.h"

#include "levels/ssl/areas/1/5/geo.inc.c"
#include "levels/ssl/areas/2/4/geo.inc.c"
#include "levels/ssl/pyramid_top/geo.inc.c"
#include "levels/ssl/tox_box/geo.inc.c"
#include "levels/ssl/areas/1/geo.inc.c"
#include "levels/ssl/grindel/geo.inc.c"
#include "levels/ssl/spindel/geo.inc.c"
#include "levels/ssl/moving_pyramid_wall/geo.inc.c"
#include "levels/ssl/pyramid_elevator/geo.inc.c"
#include "levels/ssl/eyerok_col/geo.inc.c"
#include "levels/ssl/areas/2/geo.inc.c"
#include "levels/ssl/areas/3/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(ssl_geo_0005C0);
	EXPORT_GEO(ssl_geo_0005D8);
	EXPORT_GEO(ssl_geo_000618);
	EXPORT_GEO(ssl_geo_000630);
	EXPORT_GEO(ssl_level_geo);
	EXPORT_GEO(ssl_geo_000734);
	EXPORT_GEO(ssl_geo_000764);
	EXPORT_GEO(ssl_geo_000794);
	EXPORT_GEO(ssl_geo_0007AC);
	EXPORT_GEO(ssl_pyramid_level_geo);
	EXPORT_GEO(ssl_geo_00088C);
}
