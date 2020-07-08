#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/vcutm/header.h"

#include "levels/vcutm/seesaw/geo.inc.c"
#include "levels/vcutm/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(vcutm_geo_0001F0);
	EXPORT_GEO(vcutm_level_geo);
}
