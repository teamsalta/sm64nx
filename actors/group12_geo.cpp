#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group12.h"

#include "bowser_flame/geo.inc.c"
#include "impact_ring/geo.inc.c"
#include "yellow_sphere/geo.inc.c"
#include "bowser/geo.inc.c"
#include "bomb/geo.inc.c"
#include "impact_smoke/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(bowser_flames_geo);
	EXPORT_GEO(invisible_bowser_accessory_geo);
	EXPORT_GEO(bowser_1_yellow_sphere_geo);
	EXPORT_GEO(bowser_geo_0000D8);
	EXPORT_GEO(bowser_geo_000424);
	EXPORT_GEO(bowser_geo_000770);
	EXPORT_GEO(bowser_shadow_geo);
	EXPORT_GEO(bowser_geo);
	EXPORT_GEO(bowser2_geo);
	EXPORT_GEO(bowser_bomb_geo);
	EXPORT_GEO(bowser_impact_smoke_geo);
}

