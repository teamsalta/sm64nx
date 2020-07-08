#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/hmc/header.h"

#include "levels/hmc/areas/1/grill_door/geo.inc.c"
#include "levels/hmc/rolling_rock/geo.inc.c"
#include "levels/hmc/rolling_rock_fragment_1/geo.inc.c"
#include "levels/hmc/rolling_rock_fragment_2/geo.inc.c"
#include "levels/hmc/arrow_platform/geo.inc.c"
#include "levels/hmc/arrow_platform_button/geo.inc.c"
#include "levels/hmc/elevator_platform/geo.inc.c"
#include "levels/hmc/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(hmc_geo_000530);
	EXPORT_GEO(hmc_geo_000548);
	EXPORT_GEO(hmc_geo_000570);
	EXPORT_GEO(hmc_geo_000588);
	EXPORT_GEO(hmc_geo_0005A0);
	EXPORT_GEO(hmc_geo_0005B8);
	EXPORT_GEO(hmc_geo_0005D0);
	EXPORT_GEO(hmc_geo_0005E8);
	EXPORT_GEO(hmc_geo_000618);
	EXPORT_GEO(hmc_geo_000658);
	EXPORT_GEO(hmc_geo_0006A8);
	EXPORT_GEO(hmc_geo_0006E0);
	EXPORT_GEO(hmc_geo_000700);
	EXPORT_GEO(hmc_geo_000748);
	EXPORT_GEO(hmc_geo_000770);
	EXPORT_GEO(hmc_geo_000798);
	EXPORT_GEO(hmc_geo_0007F8);
	EXPORT_GEO(hmc_geo_000850);
	EXPORT_GEO(hmc_geo_0008D0);
	EXPORT_GEO(hmc_geo_000938);
	EXPORT_GEO(hmc_geo_000998);
	EXPORT_GEO(hmc_geo_000A18);
	EXPORT_GEO(hmc_geo_000A88);
	EXPORT_GEO(hmc_geo_000AE8);
	EXPORT_GEO(hmc_geo_000B48);
	EXPORT_GEO(hmc_level_geo);
}

