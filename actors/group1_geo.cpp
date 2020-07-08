#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group1.h"

#include "yellow_sphere_small/geo.inc.c"
#include "hoot/geo.inc.c"
#include "yoshi_egg/geo.inc.c"
#include "thwomp/geo.inc.c"
#include "bullet_bill/geo.inc.c"
#include "heave_ho/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(yellow_sphere_geo);
	EXPORT_GEO(hoot_geo);
	EXPORT_GEO(yoshi_egg_geo);
	EXPORT_GEO(thwomp_geo);
	EXPORT_GEO(bullet_bill_geo);
	EXPORT_GEO(heave_ho_geo);
}
