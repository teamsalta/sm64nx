#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/castle_courtyard/header.h"

#include "levels/castle_courtyard/areas/1/spire/geo.inc.c"
#include "levels/castle_courtyard/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(castle_courtyard_geo_000200);
	EXPORT_GEO(castle_courtyard_level_geo);
}
