#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "levels/bowser_2/header.h"

#include "levels/bowser_2/tilting_platform/geo.inc.c"
#include "levels/bowser_2/areas/1/geo.inc.c"

#include "game/debug.h"
#include "engine/hook_geo.h"

namespace sm64::geo
{
	EXPORT_GEO(bowser_2_geo_000170);
	EXPORT_GEO(bowser_2_level_geo);
}
