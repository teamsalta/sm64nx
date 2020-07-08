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
#include "levels/bitdw/texture.inc.c"
#include "levels/bitdw/areas/1/1/model.inc.c"
#include "levels/bitdw/areas/1/starting_platform/model.inc.c"
#include "levels/bitdw/areas/1/large_platform/model.inc.c"
#include "levels/bitdw/areas/1/wooden_bridge_and_fences/model.inc.c"
#include "levels/bitdw/areas/1/quartzy_path_1/model.inc.c"
#include "levels/bitdw/areas/1/quartzy_path_2/model.inc.c"
#include "levels/bitdw/areas/1/quartzy_path_fences/model.inc.c"
#include "levels/bitdw/areas/1/octogonal_platform/model.inc.c"
#include "levels/bitdw/areas/1/platform_with_hill/model.inc.c"
#include "levels/bitdw/areas/1/wooden_platform/model.inc.c"
#include "levels/bitdw/areas/1/platforms_and_tilting/model.inc.c"
#include "levels/bitdw/areas/1/platforms_and_tilting_2/model.inc.c"
#include "levels/bitdw/areas/1/quartz_crystal/model.inc.c"
#include "levels/bitdw/areas/1/staircase_slope_and_platform/model.inc.c"
#include "levels/bitdw/areas/1/track_for_pyramid_platforms/model.inc.c"
#include "levels/bitdw/areas/1/narrow_path_platform/model.inc.c"
#include "levels/bitdw/sliding_platform/model.inc.c"
#include "levels/bitdw/seesaw_platform/model.inc.c"
#include "levels/bitdw/square_platform/model.inc.c"
#include "levels/bitdw/ferris_wheel_axle/model.inc.c"
#include "levels/bitdw/ferris_platform/model.inc.c"
#include "levels/bitdw/collapsing_stairs_1/model.inc.c"
#include "levels/bitdw/collapsing_stairs_2/model.inc.c"
#include "levels/bitdw/collapsing_stairs_3/model.inc.c"
#include "levels/bitdw/collapsing_stairs_4/model.inc.c"
#include "levels/bitdw/collapsing_stairs_5/model.inc.c"
#include "levels/bitdw/areas/1/collision.inc.c"
#include "levels/bitdw/areas/1/macro.inc.c"
#include "levels/bitdw/sliding_platform/collision.inc.c"
#include "levels/bitdw/seesaw_platform/collision.inc.c"
#include "levels/bitdw/square_platform/collision.inc.c"
#include "levels/bitdw/ferris_wheel_axle/collision.inc.c"
#include "levels/bitdw/ferris_platform/collision.inc.c"
#include "levels/bitdw/collapsing_stairs_1/collision.inc.c"
#include "levels/bitdw/collapsing_stairs_2/collision.inc.c"
#include "levels/bitdw/collapsing_stairs_3/collision.inc.c"
#include "levels/bitdw/collapsing_stairs_4/collision.inc.c"
#include "levels/bitdw/collapsing_stairs_5/collision.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(bitdw_seg7_dl_07002080);
	EXPORT_DL(bitdw_seg7_dl_070020C8);
	EXPORT_DL(bitdw_seg7_dl_07002698);
	EXPORT_DL(bitdw_seg7_dl_07002700);
	EXPORT_DL(bitdw_seg7_dl_07002758);
	EXPORT_DL(bitdw_seg7_dl_07002868);
	EXPORT_DL(bitdw_seg7_dl_070028A0);
	EXPORT_DL(bitdw_seg7_dl_07003088);
	EXPORT_DL(bitdw_seg7_dl_07003160);
	EXPORT_DL(bitdw_seg7_dl_070032F8);
	EXPORT_DL(bitdw_seg7_dl_07003520);
	EXPORT_DL(bitdw_seg7_dl_07003588);
	EXPORT_DL(bitdw_seg7_dl_070035D0);
	EXPORT_DL(bitdw_seg7_dl_07003608);
	EXPORT_DL(bitdw_seg7_dl_07003A98);
	EXPORT_DL(bitdw_seg7_dl_07003BF0);
	EXPORT_DL(bitdw_seg7_dl_07004160);
	EXPORT_DL(bitdw_seg7_dl_07004220);
	EXPORT_DL(bitdw_seg7_dl_07004318);
	EXPORT_DL(bitdw_seg7_dl_07004530);
	EXPORT_DL(bitdw_seg7_dl_070045C0);
	EXPORT_DL(bitdw_seg7_dl_07004DC0);
	EXPORT_DL(bitdw_seg7_dl_07004F70);
	EXPORT_DL(bitdw_seg7_dl_07004FF8);
	EXPORT_DL(bitdw_seg7_dl_07005078);
	EXPORT_DL(bitdw_seg7_dl_070058F8);
	EXPORT_DL(bitdw_seg7_dl_070059D0);
	EXPORT_DL(bitdw_seg7_dl_07005B58);
	EXPORT_DL(bitdw_seg7_dl_07005BC0);
	EXPORT_DL(bitdw_seg7_dl_07006360);
	EXPORT_DL(bitdw_seg7_dl_070063E8);
	EXPORT_DL(bitdw_seg7_dl_070064A8);
	EXPORT_DL(bitdw_seg7_dl_070065F0);
	EXPORT_DL(bitdw_seg7_dl_070075B0);
	EXPORT_DL(bitdw_seg7_dl_07007810);
	EXPORT_DL(bitdw_seg7_dl_07007940);
	EXPORT_DL(bitdw_seg7_dl_07007A18);
	EXPORT_DL(bitdw_seg7_dl_07007AA8);
	EXPORT_DL(bitdw_seg7_dl_07008AF0);
	EXPORT_DL(bitdw_seg7_dl_07008D58);
	EXPORT_DL(bitdw_seg7_dl_07008E88);
	EXPORT_DL(bitdw_seg7_dl_07008F60);
	EXPORT_DL(bitdw_seg7_dl_07008FF0);
	EXPORT_DL(bitdw_seg7_dl_070092E8);
	EXPORT_DL(bitdw_seg7_dl_070093B0);
	EXPORT_DL(bitdw_seg7_dl_07009F90);
	EXPORT_DL(bitdw_seg7_dl_0700A058);
	EXPORT_DL(bitdw_seg7_dl_0700A188);
	EXPORT_DL(bitdw_seg7_dl_0700A2B8);
	EXPORT_DL(bitdw_seg7_dl_0700A310);
	EXPORT_DL(bitdw_seg7_dl_0700A368);
	EXPORT_DL(bitdw_seg7_dl_0700A5F8);
	EXPORT_DL(bitdw_seg7_dl_0700A6A8);
	EXPORT_DL(bitdw_seg7_dl_0700AB78);
	EXPORT_DL(bitdw_seg7_dl_0700AC70);
	EXPORT_DL(bitdw_seg7_dl_0700ACC8);
	EXPORT_DL(bitdw_seg7_dl_0700AD10);
	EXPORT_DL(bitdw_seg7_dl_0700AF10);
	EXPORT_DL(bitdw_seg7_dl_0700AFA0);
	EXPORT_DL(bitdw_seg7_dl_0700B190);
	EXPORT_DL(bitdw_seg7_dl_0700B220);
	EXPORT_DL(bitdw_seg7_dl_0700B410);
	EXPORT_DL(bitdw_seg7_dl_0700B480);
	EXPORT_DL(bitdw_seg7_dl_0700B808);
	EXPORT_DL(bitdw_seg7_dl_0700B8D8);
	EXPORT_DL(bitdw_seg7_dl_0700BAC8);
	EXPORT_DL(bitdw_seg7_dl_0700BB58);
	EXPORT_DL(bitdw_seg7_dl_0700BFA8);
	EXPORT_DL(bitdw_seg7_dl_0700C0A8);
	EXPORT_DL(bitdw_seg7_dl_0700C0E0);
	EXPORT_DL(bitdw_seg7_dl_0700C538);
	EXPORT_DL(bitdw_seg7_dl_0700C638);
	EXPORT_DL(bitdw_seg7_dl_0700C670);
	EXPORT_DL(bitdw_seg7_dl_0700CAC8);
	EXPORT_DL(bitdw_seg7_dl_0700CBC8);
	EXPORT_DL(bitdw_seg7_dl_0700CC00);
	EXPORT_DL(bitdw_seg7_dl_0700D058);
	EXPORT_DL(bitdw_seg7_dl_0700D158);
	EXPORT_DL(bitdw_seg7_dl_0700D190);
	EXPORT_DL(bitdw_seg7_dl_0700D348);
	EXPORT_DL(bitdw_seg7_dl_0700D3B0);
	EXPORT_DL(bitdw_seg7_dl_0700D3E8);
}
