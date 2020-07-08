#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "moving_texture_macros.h"
#include "level_misc_macros.h"
#include "macro_preset_names.h"
#include "special_preset_names.h"
#include "textures.h"
#include "dialog_ids.h"
#include "header.h"

#include "make_const_nonconst.h"
#include "levels/wdw/texture.inc.c"
#include "levels/wdw/areas/1/1/model.inc.c"
#include "levels/wdw/areas/1/2/model.inc.c"
#include "levels/wdw/areas/2/1/model.inc.c"
#include "levels/wdw/areas/2/2/model.inc.c"
#include "levels/wdw/areas/1/3/model.inc.c"
#include "levels/wdw/double_arrows/model.inc.c" // This is weird, but the only instance of a shared area object in a level. So we're treating it as a seperate model. It does not have collision anyway so it may have been combined in both areas by some sort of optimizer at compile time.
#include "levels/wdw/square_floating_platform/model.inc.c"
#include "levels/wdw/arrow_lift/model.inc.c"
#include "levels/wdw/water_level_diamond/model.inc.c"
#include "levels/wdw/hidden_platform/model.inc.c"
#include "levels/wdw/express_elevator/model.inc.c"
#include "levels/wdw/rectangular_floating_platform/model.inc.c"
#include "levels/wdw/rotating_platform/model.inc.c"
#include "levels/wdw/areas/1/collision.inc.c"
#include "levels/wdw/areas/1/macro.inc.c"
#include "levels/wdw/areas/2/collision.inc.c"
#include "levels/wdw/areas/2/macro.inc.c"
#include "levels/wdw/square_floating_platform/collision.inc.c"
#include "levels/wdw/arrow_lift/collision.inc.c"
#include "levels/wdw/water_level_diamond/collision.inc.c" // Unused
#include "levels/wdw/hidden_platform/collision.inc.c"
#include "levels/wdw/express_elevator/collision.inc.c"
#include "levels/wdw/rectangular_floating_platform/collision.inc.c"
#include "levels/wdw/rotating_platform/collision.inc.c"
#include "levels/wdw/areas/1/movtext.inc.c"
#include "levels/wdw/areas/2/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(wdw_seg7_dl_07008160);
	EXPORT_DL(wdw_seg7_dl_07008598);
	EXPORT_DL(wdw_seg7_dl_070087B8);
	EXPORT_DL(wdw_seg7_dl_07008910);
	EXPORT_DL(wdw_seg7_dl_07008B00);
	EXPORT_DL(wdw_seg7_dl_07008C00);
	EXPORT_DL(wdw_seg7_dl_07008CA0);
	EXPORT_DL(wdw_seg7_dl_07008E08);
	EXPORT_DL(wdw_seg7_dl_07009268);
	EXPORT_DL(wdw_seg7_dl_07009358);
	EXPORT_DL(wdw_seg7_dl_07009850);
	EXPORT_DL(wdw_seg7_dl_070099B0);
	EXPORT_DL(wdw_seg7_dl_07009AB0);
	EXPORT_DL(wdw_seg7_dl_07009FB0);
	EXPORT_DL(wdw_seg7_dl_0700A0D0);
	EXPORT_DL(wdw_seg7_dl_0700A138);
	EXPORT_DL(wdw_seg7_dl_07010120);
	EXPORT_DL(wdw_seg7_dl_07010578);
	EXPORT_DL(wdw_seg7_dl_07010748);
	EXPORT_DL(wdw_seg7_dl_070107E8);
	EXPORT_DL(wdw_seg7_dl_07010AB8);
	EXPORT_DL(wdw_seg7_dl_07010C38);
	EXPORT_DL(wdw_seg7_dl_07010EA0);
	EXPORT_DL(wdw_seg7_dl_07011208);
	EXPORT_DL(wdw_seg7_dl_07011260);
	EXPORT_DL(wdw_seg7_dl_07011298);
	EXPORT_DL(wdw_seg7_dl_07011920);
	EXPORT_DL(wdw_seg7_dl_07011B08);
	EXPORT_DL(wdw_seg7_dl_07011D08);
	EXPORT_DL(wdw_seg7_dl_07011D88);
	EXPORT_DL(wdw_seg7_dl_07011DC0);
	EXPORT_DL(wdw_seg7_dl_07011E48);
	EXPORT_DL(wdw_seg7_dl_07012160);
	EXPORT_DL(wdw_seg7_dl_07012258);
	EXPORT_DL(wdw_seg7_dl_07012658);
	EXPORT_DL(wdw_seg7_dl_07012798);
	EXPORT_DL(wdw_seg7_dl_07012AD0);
	EXPORT_DL(wdw_seg7_dl_07012B48);
	EXPORT_DL(wdw_seg7_dl_07012B90);
	EXPORT_DL(wdw_seg7_dl_07012DC0);
	EXPORT_DL(wdw_seg7_dl_07012E08);
	EXPORT_DL(wdw_seg7_dl_07012E88);
	EXPORT_DL(wdw_seg7_dl_070130F8);
	EXPORT_DL(wdw_seg7_dl_070131B8);
	EXPORT_DL(wdw_seg7_dl_070133F0);
	EXPORT_DL(wdw_seg7_dl_07013490);
	EXPORT_DL(wdw_seg7_dl_070139D8);
	EXPORT_DL(wdw_seg7_dl_07013A50);
	EXPORT_DL(wdw_seg7_dl_07013B70);
	EXPORT_DL(wdw_seg7_dl_07013D80);
	EXPORT_DL(wdw_seg7_dl_07013DF8);
	EXPORT_DL(wdw_seg7_dl_07013E40);
	EXPORT_DL(wdw_seg7_dl_07014050);
	EXPORT_DL(wdw_seg7_dl_070140E0);
}
