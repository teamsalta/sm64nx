#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "moving_texture_macros.h"
#include "level_misc_macros.h"
#include "macro_preset_names.h"
#include "special_preset_names.h"
#include "textures.h"
#include "header.h"

#include "make_const_nonconst.h"
#include "levels/bitfs/texture.inc.c"
#include "levels/bitfs/areas/1/1/model.inc.c"
#include "levels/bitfs/areas/1/2/model.inc.c"
#include "levels/bitfs/areas/1/3/model.inc.c"
#include "levels/bitfs/areas/1/4/model.inc.c"
#include "levels/bitfs/areas/1/5/model.inc.c"
#include "levels/bitfs/areas/1/6/model.inc.c"
#include "levels/bitfs/areas/1/7/model.inc.c"
#include "levels/bitfs/areas/1/8/model.inc.c"
#include "levels/bitfs/areas/1/9/model.inc.c"
#include "levels/bitfs/areas/1/10/model.inc.c"
#include "levels/bitfs/areas/1/11/model.inc.c"
#include "levels/bitfs/areas/1/12/model.inc.c"
#include "levels/bitfs/areas/1/13/model.inc.c"
#include "levels/bitfs/areas/1/14/model.inc.c"
#include "levels/bitfs/areas/1/15/model.inc.c"
#include "levels/bitfs/areas/1/16/model.inc.c"
#include "levels/bitfs/areas/1/17/model.inc.c"
#include "levels/bitfs/areas/1/18/model.inc.c"
#include "levels/bitfs/areas/1/19/model.inc.c"
#include "levels/bitfs/areas/1/20/model.inc.c"
#include "levels/bitfs/elevator/model.inc.c"
#include "levels/bitfs/sinking_cage_platform/model.inc.c"
#include "levels/bitfs/sinking_cage_pole/model.inc.c"
#include "levels/bitfs/tilting_square_platform/model.inc.c"
#include "levels/bitfs/tumbling_platform_near/model.inc.c"
#include "levels/bitfs/tumbling_platform_far/model.inc.c"
#include "levels/bitfs/stretching_platform/1.inc.c"
#include "levels/bitfs/stretching_platform/2.inc.c"
#include "levels/bitfs/moving_square_platform/model.inc.c"
#include "levels/bitfs/sliding_platform/model.inc.c"
#include "levels/bitfs/platform_on_track/model.inc.c"
#include "levels/bitfs/sinking_platforms/model.inc.c"
#include "levels/bitfs/seesaw_platform/model.inc.c"
#include "levels/bitfs/areas/1/collision.inc.c"
#include "levels/bitfs/areas/1/macro.inc.c"
#include "levels/bitfs/elevator/collision.inc.c"
#include "levels/bitfs/sinking_cage_platform/collision.inc.c"
#include "levels/bitfs/tilting_square_platform/collision.inc.c"
#include "levels/bitfs/tumbling_platform_near/collision.inc.c"
#include "levels/bitfs/stretching_platform/collision.inc.c"
#include "levels/bitfs/moving_square_platform/collision.inc.c"
#include "levels/bitfs/sliding_platform/collision.inc.c"
#include "levels/bitfs/platform_on_track/collision.inc.c"
#include "levels/bitfs/sinking_platforms/collision.inc.c"
#include "levels/bitfs/seesaw_platform/collision.inc.c"
#include "levels/bitfs/areas/1/trajectory.inc.c"
#include "levels/bitfs/areas/1/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(bitfs_seg7_dl_070020C0);
	EXPORT_DL(bitfs_seg7_dl_07002118);
	EXPORT_DL(bitfs_seg7_dl_070027E8);
	EXPORT_DL(bitfs_seg7_dl_07002878);
	EXPORT_DL(bitfs_seg7_dl_07002908);
	EXPORT_DL(bitfs_seg7_dl_070029C8);
	EXPORT_DL(bitfs_seg7_dl_07002A78);
	EXPORT_DL(bitfs_seg7_dl_070033E0);
	EXPORT_DL(bitfs_seg7_dl_07003528);
	EXPORT_DL(bitfs_seg7_dl_07003670);
	EXPORT_DL(bitfs_seg7_dl_07003AB0);
	EXPORT_DL(bitfs_seg7_dl_07003AF8);
	EXPORT_DL(bitfs_seg7_dl_07003BD0);
	EXPORT_DL(bitfs_seg7_dl_07003C60);
	EXPORT_DL(bitfs_seg7_dl_07003FC8);
	EXPORT_DL(bitfs_seg7_dl_070040B0);
	EXPORT_DL(bitfs_seg7_dl_07004500);
	EXPORT_DL(bitfs_seg7_dl_07004630);
	EXPORT_DL(bitfs_seg7_dl_07006220);
	EXPORT_DL(bitfs_seg7_dl_070064A8);
	EXPORT_DL(bitfs_seg7_dl_07006790);
	EXPORT_DL(bitfs_seg7_dl_070067F8);
	EXPORT_DL(bitfs_seg7_dl_07006900);
	EXPORT_DL(bitfs_seg7_dl_07006AD8);
	EXPORT_DL(bitfs_seg7_dl_07006B90);
	EXPORT_DL(bitfs_seg7_dl_07006F58);
	EXPORT_DL(bitfs_seg7_dl_07007008);
	EXPORT_DL(bitfs_seg7_dl_07007070);
	EXPORT_DL(bitfs_seg7_dl_07007578);
	EXPORT_DL(bitfs_seg7_dl_07007700);
	EXPORT_DL(bitfs_seg7_dl_07007720);
	EXPORT_DL(bitfs_seg7_dl_070078D8);
	EXPORT_DL(bitfs_seg7_dl_07007958);
	EXPORT_DL(bitfs_seg7_dl_070089E8);
	EXPORT_DL(bitfs_seg7_dl_07008A68);
	EXPORT_DL(bitfs_seg7_dl_07008CE0);
	EXPORT_DL(bitfs_seg7_dl_07008DB8);
	EXPORT_DL(bitfs_seg7_dl_07008E20);
	EXPORT_DL(bitfs_seg7_dl_07008E68);
	EXPORT_DL(bitfs_seg7_dl_07008F18);
	EXPORT_DL(bitfs_seg7_dl_07008F48);
	EXPORT_DL(bitfs_seg7_dl_07009140);
	EXPORT_DL(bitfs_seg7_dl_07009178);
	EXPORT_DL(bitfs_seg7_dl_070091E0);
	EXPORT_DL(bitfs_seg7_dl_070094D8);
	EXPORT_DL(bitfs_seg7_dl_07009588);
	EXPORT_DL(bitfs_seg7_dl_070095E0);
	EXPORT_DL(bitfs_seg7_dl_0700A520);
	EXPORT_DL(bitfs_seg7_dl_0700A9D0);
	EXPORT_DL(bitfs_seg7_dl_0700AA00);
	EXPORT_DL(bitfs_seg7_dl_0700AB38);
	EXPORT_DL(bitfs_seg7_dl_0700AB90);
	EXPORT_DL(bitfs_seg7_dl_0700BA50);
	EXPORT_DL(bitfs_seg7_dl_0700BC38);
	EXPORT_DL(bitfs_seg7_dl_0700BD10);
	EXPORT_DL(bitfs_seg7_dl_0700BE58);
	EXPORT_DL(bitfs_seg7_dl_0700BED8);
	EXPORT_DL(bitfs_seg7_dl_0700C2B0);
	EXPORT_DL(bitfs_seg7_dl_0700C3C0);
	EXPORT_DL(bitfs_seg7_dl_0700E460);
	EXPORT_DL(bitfs_seg7_dl_0700E530);
	EXPORT_DL(bitfs_seg7_dl_0700EC78);
	EXPORT_DL(bitfs_seg7_dl_0700ED50);
	EXPORT_DL(bitfs_seg7_dl_0700ED90);
	EXPORT_DL(bitfs_seg7_dl_0700F0E0);
	EXPORT_DL(bitfs_seg7_dl_0700F1C8);
	EXPORT_DL(bitfs_seg7_dl_0700F3F8);
	EXPORT_DL(bitfs_seg7_dl_0700F430);
	EXPORT_DL(bitfs_seg7_dl_0700F498);
	EXPORT_DL(bitfs_seg7_dl_0700F4D0);
	EXPORT_DL(bitfs_seg7_dl_0700F508);
	EXPORT_DL(bitfs_seg7_dl_0700F650);
	EXPORT_DL(bitfs_seg7_dl_0700F6A8);
	EXPORT_DL(bitfs_seg7_dl_0700FA38);
	EXPORT_DL(bitfs_seg7_dl_0700FB38);
	EXPORT_DL(bitfs_seg7_dl_0700FC88);
	EXPORT_DL(bitfs_seg7_dl_0700FCC0);
	EXPORT_DL(bitfs_seg7_dl_0700FD08);
	EXPORT_DL(bitfs_seg7_dl_0700FF00);
	EXPORT_DL(bitfs_seg7_dl_0700FF90);
	EXPORT_DL(bitfs_seg7_dl_07010100);
	EXPORT_DL(bitfs_seg7_dl_07010168);
	EXPORT_DL(bitfs_seg7_dl_070102D8);
	EXPORT_DL(bitfs_seg7_dl_07010340);
	EXPORT_DL(bitfs_seg7_dl_07010E20);
	EXPORT_DL(bitfs_seg7_dl_07011138);
	EXPORT_DL(bitfs_seg7_dl_07011298);
	EXPORT_DL(bitfs_seg7_dl_070112D0);
	EXPORT_DL(bitfs_seg7_dl_07011318);
	EXPORT_DL(bitfs_seg7_dl_070114E8);
	EXPORT_DL(bitfs_seg7_dl_07011568);
	EXPORT_DL(bitfs_seg7_dl_07011718);
	EXPORT_DL(bitfs_seg7_dl_07011798);
	EXPORT_DL(bitfs_seg7_dl_07011AB8);
	EXPORT_DL(bitfs_seg7_dl_07011B48);
	EXPORT_DL(bitfs_seg7_dl_07011BA0);
	EXPORT_DL(bitfs_seg7_dl_07011D98);
	EXPORT_DL(bitfs_seg7_dl_07011E28);
	EXPORT_DL(bitfs_dl_lava_sections);
	EXPORT_DL(bitfs_dl_lava_floor);
}
