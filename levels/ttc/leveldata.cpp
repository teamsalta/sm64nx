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
// LEVEL DATA
#include "levels/ttc/texture.inc.c"
#include "levels/ttc/areas/1/1/model.inc.c"
#include "levels/ttc/areas/1/2/model.inc.c"
#include "levels/ttc/areas/1/3/model.inc.c"
#include "levels/ttc/rotating_cube/model.inc.c"
#include "levels/ttc/rotating_prism/model.inc.c"
#include "levels/ttc/pendulum/model.inc.c"
#include "levels/ttc/large_treadmill/model.inc.c"
#include "levels/ttc/small_treadmill/model.inc.c"
#include "levels/ttc/push_block/model.inc.c"
#include "levels/ttc/rotating_hexagon/model.inc.c"
#include "levels/ttc/rotating_triangle/model.inc.c"
#include "levels/ttc/pit_block/model.inc.c"
#include "levels/ttc/pit_block_2/model.inc.c"
#include "levels/ttc/elevator_platform/model.inc.c"
#include "levels/ttc/clock_hand/model.inc.c"
#include "levels/ttc/spinner/model.inc.c"
#include "levels/ttc/small_gear/model.inc.c"
#include "levels/ttc/large_gear/model.inc.c"
#include "levels/ttc/areas/1/collision.inc.c"
#include "levels/ttc/rotating_cube/collision.inc.c"
#include "levels/ttc/rotating_prism/collision.inc.c"
#include "levels/ttc/pendulum/collision.inc.c"
#include "levels/ttc/large_treadmill/collision.inc.c"
#include "levels/ttc/small_treadmill/collision.inc.c"
#include "levels/ttc/push_block/collision.inc.c"
#include "levels/ttc/rotating_hexagon/collision.inc.c"
#include "levels/ttc/rotating_triangle/collision.inc.c"
#include "levels/ttc/pit_block/collision.inc.c"
#include "levels/ttc/pit_block_2/collision.inc.c"
#include "levels/ttc/elevator_platform/collision.inc.c"
#include "levels/ttc/clock_hand/collision.inc.c"
#include "levels/ttc/spinner/collision.inc.c"
#include "levels/ttc/small_gear/collision.inc.c"
#include "levels/ttc/large_gear/collision.inc.c"
#include "levels/ttc/areas/1/macro.inc.c"
#include "levels/ttc/areas/1/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(ttc_seg7_dl_070089A8);
	EXPORT_DL(ttc_seg7_dl_07008CD8);
	EXPORT_DL(ttc_seg7_dl_07008DD0);
	EXPORT_DL(ttc_seg7_dl_07009278);
	EXPORT_DL(ttc_seg7_dl_07009480);
	EXPORT_DL(ttc_seg7_dl_07009A88);
	EXPORT_DL(ttc_seg7_dl_07009BF0);
	EXPORT_DL(ttc_seg7_dl_07009F00);
	EXPORT_DL(ttc_seg7_dl_0700A2C8);
	EXPORT_DL(ttc_seg7_dl_0700A600);
	EXPORT_DL(ttc_seg7_dl_0700A700);
	EXPORT_DL(ttc_seg7_dl_0700ACD8);
	EXPORT_DL(ttc_seg7_dl_0700AD38);
	EXPORT_DL(ttc_seg7_dl_0700B110);
	EXPORT_DL(ttc_seg7_dl_0700B1D8);
	EXPORT_DL(ttc_seg7_dl_0700DC98);
	EXPORT_DL(ttc_seg7_dl_0700E530);
	EXPORT_DL(ttc_seg7_dl_0700E878);
	EXPORT_DL(ttc_seg7_dl_0700EBC0);
	EXPORT_DL(ttc_seg7_dl_0700EC50);
	EXPORT_DL(ttc_seg7_dl_0700ECB8);
	EXPORT_DL(ttc_seg7_dl_0700EF20);
	EXPORT_DL(ttc_seg7_dl_0700EF88);
	EXPORT_DL(ttc_seg7_dl_0700EFE0);
	EXPORT_DL(ttc_seg7_dl_0700F528);
	EXPORT_DL(ttc_seg7_dl_0700F5A0);
	EXPORT_DL(ttc_seg7_dl_0700F670);
	EXPORT_DL(ttc_seg7_dl_0700F760);
	EXPORT_DL(ttc_seg7_dl_0700FAA0);
	EXPORT_DL(ttc_seg7_dl_0700FAF8);
	EXPORT_DL(ttc_seg7_dl_0700FB88);
	EXPORT_DL(ttc_seg7_dl_0700FBB8);
	EXPORT_DL(ttc_seg7_dl_0700FED0);
	EXPORT_DL(ttc_seg7_dl_0700FF28);
	EXPORT_DL(ttc_seg7_dl_0700FFB8);
	EXPORT_DL(ttc_seg7_dl_0700FFE8);
	EXPORT_DL(ttc_seg7_dl_07010208);
	EXPORT_DL(ttc_seg7_dl_07010280);
	EXPORT_DL(ttc_seg7_dl_070102B8);
	EXPORT_DL(ttc_seg7_dl_07010738);
	EXPORT_DL(ttc_seg7_dl_070107D8);
	EXPORT_DL(ttc_seg7_dl_07010868);
	EXPORT_DL(ttc_seg7_dl_07010BF8);
	EXPORT_DL(ttc_seg7_dl_07010CF0);
	EXPORT_DL(ttc_seg7_dl_07010D38);
	EXPORT_DL(ttc_seg7_dl_07010F80);
	EXPORT_DL(ttc_seg7_dl_07010FD8);
	EXPORT_DL(ttc_seg7_dl_07011040);
	EXPORT_DL(ttc_seg7_dl_070112A0);
	EXPORT_DL(ttc_seg7_dl_070112F8);
	EXPORT_DL(ttc_seg7_dl_07011360);
	EXPORT_DL(ttc_seg7_dl_070115E0);
	EXPORT_DL(ttc_seg7_dl_07011670);
	EXPORT_DL(ttc_seg7_dl_070116A8);
	EXPORT_DL(ttc_seg7_dl_070119F8);
	EXPORT_DL(ttc_seg7_dl_07011B38);
	EXPORT_DL(ttc_seg7_dl_07011EC0);
	EXPORT_DL(ttc_seg7_dl_07012028);
	EXPORT_DL(ttc_seg7_dl_07012110);
	EXPORT_DL(ttc_seg7_dl_07012148);
	EXPORT_DL(ttc_seg7_dl_07012240);
	EXPORT_DL(ttc_seg7_dl_07012278);
	EXPORT_DL(ttc_dl_surface_treadmill_begin);
	EXPORT_DL(ttc_dl_surface_treadmill_end);
	EXPORT_DL(ttc_dl_surface_treadmill);
}
