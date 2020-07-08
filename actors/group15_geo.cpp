#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group15.h"

#include "lakitu_cameraman/geo.inc.c"
#include "toad/geo.inc.c"
#include "mips/geo.inc.c"
#include "boo_castle/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(lakitu_geo);
	EXPORT_GEO(toad_geo_000114);
	EXPORT_GEO(toad_geo_00027C);
	EXPORT_GEO(toad_geo);
	EXPORT_GEO(mips_geo);
	EXPORT_GEO(boo_castle_geo);
}
