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
#include "levels/bob/texture.inc.c"
#include "levels/bob/areas/1/1/model.inc.c"
#include "levels/bob/areas/1/2/model.inc.c"
#include "levels/bob/areas/1/3/model.inc.c"
#include "levels/bob/areas/1/4/model.inc.c"
#include "levels/bob/areas/1/5/model.inc.c"
#include "levels/bob/areas/1/6/model.inc.c"
#include "levels/bob/chain_chomp_gate/model.inc.c"
#include "levels/bob/seesaw_platform/model.inc.c"
#include "levels/bob/grate_door/model.inc.c"
#include "levels/bob/areas/1/collision.inc.c"
#include "levels/bob/areas/1/macro.inc.c"
#include "levels/bob/chain_chomp_gate/collision.inc.c"
#include "levels/bob/seesaw_platform/collision.inc.c"
#include "levels/bob/grate_door/collision.inc.c"
#include "levels/bob/areas/1/trajectory.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(bob_seg7_dl_07003CA8);
	EXPORT_DL(bob_seg7_dl_070041E0);
	EXPORT_DL(bob_seg7_dl_070042B8);
	EXPORT_DL(bob_seg7_dl_07004390);
	EXPORT_DL(bob_seg7_dl_07008AF0);
	EXPORT_DL(bob_seg7_dl_07008C28);
	EXPORT_DL(bob_seg7_dl_07009050);
	EXPORT_DL(bob_seg7_dl_07009370);
	EXPORT_DL(bob_seg7_dl_07009490);
	EXPORT_DL(bob_seg7_dl_070095B8);
	EXPORT_DL(bob_seg7_dl_07009768);
	EXPORT_DL(bob_seg7_dl_070097F8);
	EXPORT_DL(bob_seg7_dl_07009960);
	EXPORT_DL(bob_seg7_dl_070099E0);
	EXPORT_DL(bob_seg7_dl_07009D80);
	EXPORT_DL(bob_seg7_dl_0700A318);
	EXPORT_DL(bob_seg7_dl_0700A470);
	EXPORT_DL(bob_seg7_dl_0700A800);
	EXPORT_DL(bob_seg7_dl_0700A848);
	EXPORT_DL(bob_seg7_dl_0700A920);
	EXPORT_DL(bob_seg7_dl_0700CFC0);
	EXPORT_DL(bob_seg7_dl_0700D7D8);
	EXPORT_DL(bob_seg7_dl_0700D910);
	EXPORT_DL(bob_seg7_dl_0700DC40);
	EXPORT_DL(bob_seg7_dl_0700DC88);
	EXPORT_DL(bob_seg7_dl_0700DCE0);
	EXPORT_DL(bob_seg7_dl_0700DD18);
	EXPORT_DL(bob_seg7_dl_0700E1E8);
	EXPORT_DL(bob_seg7_dl_0700E338);
	EXPORT_DL(bob_seg7_dl_0700E420);
	EXPORT_DL(bob_seg7_dl_0700E458);
	EXPORT_DL(bob_seg7_dl_0700E6C8);
	EXPORT_DL(bob_seg7_dl_0700E768);
	EXPORT_DL(bob_seg7_dl_0700E860);
	EXPORT_DL(bob_seg7_dl_0700E8A0);
}
