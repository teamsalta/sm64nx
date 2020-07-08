#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group7.h"

#include "spindrift/geo.inc.c"
#include "penguin/geo.inc.c"
#include "snowman/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(spindrift_geo);
	EXPORT_GEO(penguin_geo);
	EXPORT_GEO(mr_blizzard_hidden_geo);
	EXPORT_GEO(mr_blizzard_geo);
}
