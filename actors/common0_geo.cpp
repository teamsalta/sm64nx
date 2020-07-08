#include <ultra64.h>
#include "sm64.h"
#include "game/segment2.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"


#include "common1.h"
#include "common0.h"

#include "blue_coin_switch/geo.inc.c"
// Hmm, this is included earlier.
#include "test_platform/geo.inc.c"
#include "amp/geo.inc.c"
#include "cannon_lid/geo.inc.c"
#include "cannon_base/geo.inc.c"
#include "cannon_barrel/geo.inc.c"
#include "chuckya/geo.inc.c"
#include "purple_switch/geo.inc.c"
#include "checkerboard_platform/geo.inc.c"
#include "heart/geo.inc.c"
#include "flyguy/geo.inc.c"
#include "breakable_box/geo.inc.c"
#include "bowling_ball/geo.inc.c"
#include "exclamation_box/geo.inc.c"
#include "goomba/geo.inc.c"
#include "bobomb/geo.inc.c"
#include "metal_box/geo.inc.c"
#include "exclamation_box_outline/geo.inc.c"
#include "koopa_shell/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(blue_coin_switch_geo);
	EXPORT_GEO(amp_geo);
	EXPORT_GEO(cannon_base_geo);
	EXPORT_GEO(cannon_barrel_geo);
	EXPORT_GEO(chuckya_geo);
	EXPORT_GEO(purple_switch_geo);
	EXPORT_GEO(checkerboard_platform_geo);
	EXPORT_GEO(heart_geo);
	EXPORT_GEO(flyguy_geo);
	EXPORT_GEO(breakable_box_geo);
	EXPORT_GEO(breakable_box_small_geo);
	EXPORT_GEO(bowling_ball_geo);
	EXPORT_GEO(bowling_ball_track_geo);
	EXPORT_GEO(exclamation_box_geo);
	EXPORT_GEO(goomba_geo);
	EXPORT_GEO(black_bobomb_geo);
	EXPORT_GEO(bobomb_buddy_geo);
	EXPORT_GEO(metal_box_geo);
	EXPORT_GEO(exclamation_box_outline_geo);
	EXPORT_GEO(koopa_shell_geo);
	EXPORT_GEO(koopa_shell2_geo);
	EXPORT_GEO(koopa_shell3_geo);
}
