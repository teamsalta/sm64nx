#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group8.h"

#include "springboard/geo.inc.c"
#include "capswitch/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(springboard_top_geo);
	EXPORT_GEO(springboard_spring_geo);
	EXPORT_GEO(springboard_bottom_geo);
	EXPORT_GEO(cap_switch_geo);
}
