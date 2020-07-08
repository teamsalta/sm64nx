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
#include "levels/ddd/texture.inc.c"
#include "levels/ddd/areas/1/1/model.inc.c"
#include "levels/ddd/areas/1/2/model.inc.c"
#include "levels/ddd/areas/1/3/model.inc.c"
#include "levels/ddd/areas/1/4/model.inc.c"
#include "levels/ddd/areas/2/1/model.inc.c"
#include "levels/ddd/areas/2/2/model.inc.c"
#include "levels/ddd/areas/2/3/model.inc.c"
#include "levels/ddd/areas/2/4/model.inc.c"
#include "levels/ddd/sub_door/1.inc.c"
#include "levels/ddd/sub_door/2.inc.c"
#include "levels/ddd/sub_door/3.inc.c"
#include "levels/ddd/submarine/1.inc.c"
#include "levels/ddd/submarine/2.inc.c"
#include "levels/ddd/areas/2/5/model.inc.c"
#include "levels/ddd/areas/2/6/model.inc.c"
#include "levels/ddd/pole/model.inc.c"
#include "levels/ddd/areas/1/collision.inc.c"
#include "levels/ddd/areas/2/collision.inc.c"
#include "levels/ddd/areas/1/macro.inc.c"
#include "levels/ddd/areas/2/macro.inc.c"
#include "levels/ddd/submarine/collision.inc.c"
#include "levels/ddd/sub_door/collision.inc.c"
#include "levels/ddd/areas/1/movtext.inc.c"
#include "levels/ddd/areas/2/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(ddd_seg7_dl_07004700);
	EXPORT_DL(ddd_seg7_dl_07004D48);
	EXPORT_DL(ddd_seg7_dl_07005570);
	EXPORT_DL(ddd_seg7_dl_07005738);
	EXPORT_DL(ddd_seg7_dl_070057E8);
	EXPORT_DL(ddd_seg7_dl_07005AF8);
	EXPORT_DL(ddd_seg7_dl_07005C40);
	EXPORT_DL(ddd_seg7_dl_07005EE0);
	EXPORT_DL(ddd_seg7_dl_07005F78);
	EXPORT_DL(ddd_seg7_dl_07006E20);
	EXPORT_DL(ddd_seg7_dl_07006EE0);
	EXPORT_DL(ddd_seg7_dl_07007080);
	EXPORT_DL(ddd_seg7_dl_07007298);
	EXPORT_DL(ddd_seg7_dl_07007408);
	EXPORT_DL(ddd_seg7_dl_07007AF0);
	EXPORT_DL(ddd_seg7_dl_07007CB8);
	EXPORT_DL(ddd_seg7_dl_070088F0);
	EXPORT_DL(ddd_seg7_dl_07008C48);
	EXPORT_DL(ddd_seg7_dl_07008EE8);
	EXPORT_DL(ddd_seg7_dl_07008F80);
	EXPORT_DL(ddd_seg7_dl_07009010);
	EXPORT_DL(ddd_seg7_dl_07009030);
	EXPORT_DL(ddd_seg7_dl_070090D8);
	EXPORT_DL(ddd_seg7_dl_07009120);
	EXPORT_DL(ddd_seg7_dl_070091D0);
	EXPORT_DL(ddd_seg7_dl_07009208);
	EXPORT_DL(ddd_seg7_dl_0700A600);
	EXPORT_DL(ddd_seg7_dl_0700AEB8);
	EXPORT_DL(ddd_seg7_dl_0700AF10);
	EXPORT_DL(ddd_seg7_dl_0700B010);
	EXPORT_DL(ddd_seg7_dl_0700B068);
	EXPORT_DL(ddd_seg7_dl_0700B890);
	EXPORT_DL(ddd_seg7_dl_0700BAE0);
	EXPORT_DL(ddd_seg7_dl_0700C9D8);
	EXPORT_DL(ddd_seg7_dl_0700CA70);
	EXPORT_DL(ddd_seg7_dl_0700CC70);
	EXPORT_DL(ddd_seg7_dl_0700CE48);
	EXPORT_DL(ddd_seg7_dl_0700D1B8);
	EXPORT_DL(ddd_seg7_dl_0700D2A0);
}
