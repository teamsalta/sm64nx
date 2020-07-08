#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group14.h"

#include "koopa_flag/geo.inc.c"
#include "poundable_pole/geo.inc.c"
#include "koopa/geo.inc.c"
#include "piranha_plant/geo.inc.c"
#include "whomp/geo.inc.c"
#include "chain_ball/geo.inc.c"
#include "chain_chomp/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(koopa_flag_geo);
	EXPORT_GEO(wooden_post_geo);
	EXPORT_GEO(koopa_without_shell_geo);
	EXPORT_GEO(koopa_with_shell_geo);
	EXPORT_GEO(piranha_plant_geo);
	EXPORT_GEO(whomp_geo);
	EXPORT_GEO(metallic_ball_geo);
	EXPORT_GEO(chain_chomp_geo);
}
