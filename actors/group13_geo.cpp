#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group13.h"

#include "skeeter/geo.inc.c"
#include "seaweed/geo.inc.c"
#include "water_mine/geo.inc.c"
#include "cyan_fish/geo.inc.c"
#include "bub/geo.inc.c"
#include "water_ring/geo.inc.c"
#include "treasure_chest/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(skeeter_geo);
	EXPORT_GEO(seaweed_geo);
	EXPORT_GEO(water_mine_geo);
	EXPORT_GEO(cyan_fish_geo);
	EXPORT_GEO(bub_geo);
	EXPORT_GEO(water_ring_geo);
	EXPORT_GEO(treasure_chest_base_geo);
	EXPORT_GEO(treasure_chest_lid_geo);
}
