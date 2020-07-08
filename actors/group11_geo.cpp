#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group11.h"

#include "bubba/geo.inc.c"
#include "wiggler_head/geo.inc.c"
#include "lakitu_enemy/geo.inc.c"
#include "spiny_egg/geo.inc.c"
#include "spiny/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(bubba_geo);
	EXPORT_GEO(wiggler_head_geo);
	EXPORT_GEO(enemy_lakitu_geo);
	EXPORT_GEO(spiny_ball_geo);
	EXPORT_GEO(spiny_geo);
}
