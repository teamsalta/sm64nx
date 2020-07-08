#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group9.h"

#include "bookend/geo.inc.c"
#include "book/geo.inc.c"
#include "chair/geo.inc.c"
#include "small_key/geo.inc.c"
#include "mad_piano/geo.inc.c"
#include "boo/geo.inc.c"
#include "haunted_cage/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(bookend_part_geo);
	EXPORT_GEO(bookend_geo);
	EXPORT_GEO(haunted_chair_geo);
	EXPORT_GEO(small_key_geo);
	EXPORT_GEO(mad_piano_geo);
	EXPORT_GEO(boo_geo);
	EXPORT_GEO(haunted_cage_geo);
}
