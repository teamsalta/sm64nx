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
#include "levels/jrb/texture.inc.c"
#include "levels/jrb/areas/1/1/model.inc.c"
#include "levels/jrb/areas/1/2/model.inc.c"
#include "levels/jrb/areas/1/3/model.inc.c"
#include "levels/jrb/areas/1/4/model.inc.c"
#include "levels/jrb/areas/1/5/model.inc.c"
#include "levels/jrb/areas/1/6/model.inc.c"
#include "levels/jrb/areas/1/7/model.inc.c"
#include "levels/jrb/rock/model.inc.c"
#include "levels/jrb/floating_platform/model.inc.c"
#include "levels/jrb/sliding_box/model.inc.c"
#include "levels/jrb/wooden_ship/model.inc.c"
#include "levels/jrb/falling_pillar/model.inc.c"
#include "levels/jrb/falling_pillar_base/model.inc.c"
#include "levels/jrb/areas/1/collision.inc.c"
#include "levels/jrb/areas/1/macro.inc.c"
#include "levels/jrb/rock/collision.inc.c"
#include "levels/jrb/floating_platform/collision.inc.c"
#include "levels/jrb/sliding_box/collision.inc.c"
#include "levels/jrb/wooden_ship/collision.inc.c"
#include "levels/jrb/falling_pillar_base/collision.inc.c"
#include "levels/jrb/unused_lid/collision.inc.c"
#include "levels/jrb/areas/1/trajectory.inc.c"
#include "levels/jrb/areas/1/movtext.inc.c"
#include "levels/jrb/areas/2/1/model.inc.c"
#include "levels/jrb/areas/2/2/model.inc.c"
#include "levels/jrb/areas/2/3/model.inc.c"
#include "levels/jrb/areas/2/collision.inc.c"
#include "levels/jrb/areas/2/macro.inc.c"
#include "levels/jrb/areas/2/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(jrb_seg7_dl_07002DA0);
	EXPORT_DL(jrb_seg7_dl_07002E68);
	EXPORT_DL(jrb_seg7_dl_07002FD0);
	EXPORT_DL(jrb_seg7_dl_070041F0);
	EXPORT_DL(jrb_seg7_dl_070044C8);
	EXPORT_DL(jrb_seg7_dl_07004658);
	EXPORT_DL(jrb_seg7_dl_070046C0);
	EXPORT_DL(jrb_seg7_dl_07004940);
	EXPORT_DL(jrb_seg7_dl_07004B88);
	EXPORT_DL(jrb_seg7_dl_07004BF0);
	EXPORT_DL(jrb_seg7_dl_07004C78);
	EXPORT_DL(jrb_seg7_dl_07005560);
	EXPORT_DL(jrb_seg7_dl_07005660);
	EXPORT_DL(jrb_seg7_dl_070058C8);
	EXPORT_DL(jrb_seg7_dl_07006508);
	EXPORT_DL(jrb_seg7_dl_070065A8);
	EXPORT_DL(jrb_seg7_dl_070067F0);
	EXPORT_DL(jrb_seg7_dl_07006980);
	EXPORT_DL(jrb_seg7_dl_070069B0);
	EXPORT_DL(jrb_seg7_dl_070072A0);
	EXPORT_DL(jrb_seg7_dl_070074A8);
	EXPORT_DL(jrb_seg7_dl_07007500);
	EXPORT_DL(jrb_seg7_dl_07007570);
	EXPORT_DL(jrb_seg7_dl_070076A8);
	EXPORT_DL(jrb_seg7_dl_070076E0);
	EXPORT_DL(jrb_seg7_dl_07007718);
	EXPORT_DL(jrb_seg7_dl_070079E8);
	EXPORT_DL(jrb_seg7_dl_07007AC8);
	EXPORT_DL(jrb_seg7_dl_07007D08);
	EXPORT_DL(jrb_seg7_dl_07007D80);
	EXPORT_DL(jrb_seg7_dl_07007DC8);
	EXPORT_DL(jrb_seg7_dl_07008038);
	EXPORT_DL(jrb_seg7_dl_070080B0);
	EXPORT_DL(jrb_seg7_dl_070080F8);
	EXPORT_DL(jrb_seg7_dl_07008B88);
	EXPORT_DL(jrb_seg7_dl_07008C90);
	EXPORT_DL(jrb_seg7_dl_07008DD0);
	EXPORT_DL(jrb_seg7_dl_07008F70);
	EXPORT_DL(jrb_seg7_dl_07008FD8);
	EXPORT_DL(jrb_seg7_dl_070090B0);
	EXPORT_DL(jrb_seg7_dl_07009798);
	EXPORT_DL(jrb_seg7_dl_07009808);
	EXPORT_DL(jrb_seg7_dl_070098C0);
	EXPORT_DL(jrb_seg7_dl_07009A10);
	EXPORT_DL(jrb_seg7_dl_07009A58);
	EXPORT_DL(jrb_seg7_dl_07009B30);
	EXPORT_DL(jrb_seg7_dl_0700A2F8);
	EXPORT_DL(jrb_seg7_dl_0700A3A8);
	EXPORT_DL(jrb_seg7_dl_0700A470);
	EXPORT_DL(jrb_seg7_dl_0700A608);
	EXPORT_DL(jrb_seg7_dl_0700AA98);
	EXPORT_DL(jrb_seg7_dl_0700AAE0);
	EXPORT_DL(jrb_seg7_dl_0700AB60);
	EXPORT_DL(jrb_seg7_dl_0700AC68);
	EXPORT_DL(jrb_seg7_dl_0700ADD0);
	EXPORT_DL(jrb_seg7_dl_0700AE48);
	EXPORT_DL(jrb_seg7_dl_0700AF58);
	EXPORT_DL(jrb_seg7_dl_0700AFB0);
	EXPORT_DL(jrb_seg7_dl_0700E6E8);
	EXPORT_DL(jrb_seg7_dl_0700E8C8);
	EXPORT_DL(jrb_seg7_dl_0700E998);
	EXPORT_DL(jrb_seg7_dl_0700ECB8);
	EXPORT_DL(jrb_seg7_dl_0700EE28);
	EXPORT_DL(jrb_seg7_dl_0700EF00);
	EXPORT_DL(jrb_seg7_dl_0700FB30);
	EXPORT_DL(jrb_seg7_dl_0700FE48);
	EXPORT_DL(jrb_seg7_dl_07010390);
	EXPORT_DL(jrb_seg7_dl_07010548);
}
