#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group17.h"

#include "mr_i_eyeball/geo.inc.c"
#include "mr_i_iris/geo.inc.c"
#include "swoop/geo.inc.c"
#include "snufit/geo.inc.c"
#include "dorrie/geo.inc.c"
#include "scuttlebug/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(mr_i_geo);
	EXPORT_GEO(mr_i_iris_geo);
	EXPORT_GEO(swoop_geo);
	EXPORT_GEO(snufit_geo);
	EXPORT_GEO(dorrie_geo);
	EXPORT_GEO(scuttlebug_geo);
}
