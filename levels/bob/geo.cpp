#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/bob/header.h"

#include "levels/bob/chain_chomp_gate/geo.inc.c"
#include "levels/bob/seesaw_platform/geo.inc.c"
#include "levels/bob/grate_door/geo.inc.c"
#include "levels/bob/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(bob_geo_000440);
	EXPORT_GEO(bob_geo_000458);
	EXPORT_GEO(bob_geo_000470);
	EXPORT_GEO(bob_level_geo);
}
