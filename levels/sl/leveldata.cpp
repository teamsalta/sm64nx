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
#include "levels/sl/texture.inc.c"
#include "levels/sl/areas/1/1/model.inc.c"
#include "levels/sl/areas/1/2/model.inc.c"
#include "levels/sl/areas/1/3/model.inc.c"
#include "levels/sl/areas/1/4/model.inc.c"
#include "levels/sl/areas/1/5/model.inc.c"
#include "levels/sl/areas/1/6/model.inc.c"
#include "levels/sl/areas/1/7/model.inc.c"
#include "levels/sl/snow_mound/model.inc.c"
#include "levels/sl/unused_cracked_ice/model.inc.c"
#include "levels/sl/unused_ice_shard/model.inc.c" // Used in the size behavior when ground pounding the cracked ice show in the above model.
#include "levels/sl/areas/2/1/model.inc.c"
#include "levels/sl/areas/2/2/model.inc.c"
#include "levels/sl/areas/2/3/model.inc.c"
#include "levels/sl/areas/2/4/model.inc.c"
#include "levels/sl/areas/1/collision.inc.c"
#include "levels/sl/areas/1/macro.inc.c"
#include "levels/sl/snow_mound/collision.inc.c"
#include "levels/sl/unused_cracked_ice/collision.inc.c"
#include "levels/sl/areas/2/collision.inc.c"
#include "levels/sl/areas/2/macro.inc.c"
#include "levels/sl/areas/1/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(sl_seg7_dl_07004830);
	EXPORT_DL(sl_seg7_dl_07005370);
	EXPORT_DL(sl_seg7_dl_070053C8);
	EXPORT_DL(sl_seg7_dl_07005410);
	EXPORT_DL(sl_seg7_dl_07005478);
	EXPORT_DL(sl_seg7_dl_07005638);
	EXPORT_DL(sl_seg7_dl_070056B0);
	EXPORT_DL(sl_seg7_dl_07006D70);
	EXPORT_DL(sl_seg7_dl_07006DE8);
	EXPORT_DL(sl_seg7_dl_07006EB0);
	EXPORT_DL(sl_seg7_dl_070070A8);
	EXPORT_DL(sl_seg7_dl_07007118);
	EXPORT_DL(sl_seg7_dl_07007368);
	EXPORT_DL(sl_seg7_dl_070073D0);
	EXPORT_DL(sl_seg7_dl_07007770);
	EXPORT_DL(sl_seg7_dl_070077F0);
	EXPORT_DL(sl_seg7_dl_07007880);
	EXPORT_DL(sl_seg7_dl_07008528);
	EXPORT_DL(sl_seg7_dl_070088B0);
	EXPORT_DL(sl_seg7_dl_07008C30);
	EXPORT_DL(sl_seg7_dl_07008D10);
	EXPORT_DL(sl_seg7_dl_07008D58);
	EXPORT_DL(sl_seg7_dl_07009F30);
	EXPORT_DL(sl_seg7_dl_07009FE0);
	EXPORT_DL(sl_seg7_dl_0700A058);
	EXPORT_DL(sl_seg7_dl_0700A418);
	EXPORT_DL(sl_seg7_dl_0700A450);
	EXPORT_DL(sl_seg7_dl_0700A5A0);
	EXPORT_DL(sl_seg7_dl_0700A718);
	EXPORT_DL(sl_seg7_dl_0700A780);
	EXPORT_DL(sl_seg7_dl_0700A848);
	EXPORT_DL(sl_seg7_dl_0700A890);
	EXPORT_DL(sl_seg7_dl_0700B6C0);
	EXPORT_DL(sl_seg7_dl_0700BA70);
	EXPORT_DL(sl_seg7_dl_0700BAE8);
	EXPORT_DL(sl_seg7_dl_0700BC80);
	EXPORT_DL(sl_seg7_dl_0700BCF8);
	EXPORT_DL(sl_seg7_dl_0700C720);
	EXPORT_DL(sl_seg7_dl_0700C9E8);
	EXPORT_DL(sl_seg7_dl_0700CB00);
	EXPORT_DL(sl_seg7_dl_0700CB58);
}
