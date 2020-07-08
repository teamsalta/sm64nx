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
#include "levels/thi/texture.inc.c"
#include "levels/thi/areas/1/1/model.inc.c" // Area 2 defines a share path to load these DLs as part of its geo script.
#include "levels/thi/areas/1/2/model.inc.c"
#include "levels/thi/areas/1/3/model.inc.c"
#include "levels/thi/areas/1/4/model.inc.c"
#include "levels/thi/areas/1/5/model.inc.c"
#include "levels/thi/areas/2/1/model.inc.c" // This looks a little weird being here but look at the usage and it makes sense.
#include "levels/thi/areas/1/6/model.inc.c"
#include "levels/thi/areas/1/7/model.inc.c"
#include "levels/thi/areas/3/1/model.inc.c"
#include "levels/thi/areas/3/2/model.inc.c"
#include "levels/thi/areas/3/3/model.inc.c"
#include "levels/thi/areas/3/4/model.inc.c"
#include "levels/thi/areas/1/collision.inc.c"
#include "levels/thi/areas/2/collision.inc.c"
#include "levels/thi/areas/3/collision.inc.c"
#include "levels/thi/areas/1/macro.inc.c"
#include "levels/thi/areas/2/macro.inc.c"
#include "levels/thi/areas/3/macro.inc.c"
#include "levels/thi/areas/1/6/collision.inc.c"
#include "levels/thi/areas/1/trajectory.inc.c"
#include "levels/thi/areas/1/movtext.inc.c"
#include "levels/thi/areas/2/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(thi_seg7_dl_070063C0);
	EXPORT_DL(thi_seg7_dl_07006660);
	EXPORT_DL(thi_seg7_dl_07006770);
	EXPORT_DL(thi_seg7_dl_07006800);
	EXPORT_DL(thi_seg7_dl_07006868);
	EXPORT_DL(thi_seg7_dl_07006908);
	EXPORT_DL(thi_seg7_dl_07006968);
	EXPORT_DL(thi_seg7_dl_07006EB0);
	EXPORT_DL(thi_seg7_dl_07007008);
	EXPORT_DL(thi_seg7_dl_07007240);
	EXPORT_DL(thi_seg7_dl_070072E8);
	EXPORT_DL(thi_seg7_dl_07007388);
	EXPORT_DL(thi_seg7_dl_070073C0);
	EXPORT_DL(thi_seg7_dl_07007600);
	EXPORT_DL(thi_seg7_dl_07007648);
	EXPORT_DL(thi_seg7_dl_07007828);
	EXPORT_DL(thi_seg7_dl_07007870);
	EXPORT_DL(thi_seg7_dl_070078D8);
	EXPORT_DL(thi_seg7_dl_07007930);
	EXPORT_DL(thi_seg7_dl_07007B80);
	EXPORT_DL(thi_seg7_dl_07007C20);
	EXPORT_DL(thi_seg7_dl_07008F98);
	EXPORT_DL(thi_seg7_dl_07009010);
	EXPORT_DL(thi_seg7_dl_07009080);
	EXPORT_DL(thi_seg7_dl_07009530);
	EXPORT_DL(thi_seg7_dl_070095A8);
	EXPORT_DL(thi_seg7_dl_07009610);
	EXPORT_DL(thi_seg7_dl_07009670);
	EXPORT_DL(thi_seg7_dl_07009B70);
	EXPORT_DL(thi_seg7_dl_07009C40);
	EXPORT_DL(thi_seg7_dl_07009CF8);
	EXPORT_DL(thi_seg7_dl_07009D30);
	EXPORT_DL(thi_seg7_dl_07009D50);
	EXPORT_DL(thi_seg7_dl_07009EB0);
	EXPORT_DL(thi_seg7_dl_07009F58);
}
