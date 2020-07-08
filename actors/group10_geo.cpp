#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group10.h"

#include "bird/geo.inc.c"
#include "peach/geo.inc.c"
#include "yoshi/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(birds_geo);
	EXPORT_GEO(peach_geo_000098);
	EXPORT_GEO(peach_geo_000254);
	EXPORT_GEO(peach_geo);
	EXPORT_GEO(yoshi_geo);
}
