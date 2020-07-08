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
#include "levels/lll/texture.inc.c"
#include "levels/lll/areas/1/light.inc.c" // What the hell? Every level up until LLL hasn't needed this. Some models share lights, so we'll assume its a shared area file for level optimization.
#include "levels/lll/areas/1/1/model.inc.c"
#include "levels/lll/areas/1/2/model.inc.c"
#include "levels/lll/areas/1/3/model.inc.c"
#include "levels/lll/areas/1/4/model.inc.c"
#include "levels/lll/areas/1/5/model.inc.c"
#include "levels/lll/areas/1/6/model.inc.c"
#include "levels/lll/areas/1/7/model.inc.c"
#include "levels/lll/areas/1/8/model.inc.c"
#include "levels/lll/areas/1/9/model.inc.c"
#include "levels/lll/areas/1/10/model.inc.c"
#include "levels/lll/areas/1/11/model.inc.c"
#include "levels/lll/areas/1/12/model.inc.c"
#include "levels/lll/areas/1/13/model.inc.c"
#include "levels/lll/areas/1/14/model.inc.c"
#include "levels/lll/areas/1/15/model.inc.c"
#include "levels/lll/moving_octagonal_mesh_platform/model.inc.c"
#include "levels/lll/drawbridge_part/model.inc.c"
#include "levels/lll/rotating_block_fire_bars/model.inc.c"
#include "levels/lll/wooden_float_small/model.inc.c"
#include "levels/lll/wooden_float_large/model.inc.c"
#include "levels/lll/collapsing_wooden_platform/model.inc.c"
#include "levels/lll/long_wooden_bridge/model.inc.c"
#include "levels/lll/rotating_hexagonal_ring/model.inc.c"
#include "levels/lll/sinking_rectangular_platform/model.inc.c"
#include "levels/lll/sinking_square_platform/model.inc.c"
#include "levels/lll/tilting_square_platform/model.inc.c"
#include "levels/lll/puzzle_piece/model.inc.c"
#include "levels/lll/sinking_rock_block/model.inc.c"
#include "levels/lll/rolling_log/model.inc.c"
#include "levels/lll/areas/1/collision.inc.c"
#include "levels/lll/areas/1/macro.inc.c"
#include "levels/lll/moving_octagonal_mesh_platform/collision.inc.c"
#include "levels/lll/drawbridge_part/collision.inc.c"
#include "levels/lll/rotating_block_fire_bars/collision.inc.c"
#include "levels/lll/wooden_float_small/collision.inc.c"
#include "levels/lll/collapsing_wooden_platform/collision.inc.c"
#include "levels/lll/rotating_hexagonal_ring/collision.inc.c"
#include "levels/lll/sinking_rectangular_platform/collision.inc.c"
#include "levels/lll/sinking_square_platform/collision.inc.c"
#include "levels/lll/tilting_square_platform/collision.inc.c"
#include "levels/lll/puzzle_piece/collision.inc.c"
#include "levels/lll/sinking_rock_block/collision.inc.c"
#include "levels/lll/rolling_log/collision.inc.c"
#include "levels/lll/areas/1/10/collision.inc.c"
#include "levels/lll/areas/2/1/model.inc.c"
#include "levels/lll/areas/2/2/model.inc.c"
#include "levels/lll/areas/2/3/model.inc.c"
#include "levels/lll/areas/2/4/model.inc.c"
#include "levels/lll/areas/2/5/model.inc.c"
#include "levels/lll/volcano_falling_trap/model.inc.c"
#include "levels/lll/areas/2/collision.inc.c"
#include "levels/lll/areas/2/macro.inc.c"
#include "levels/lll/volcano_falling_trap/collision.inc.c"
#include "levels/lll/areas/2/trajectory.inc.c"
#include "levels/lll/areas/2/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(lll_seg7_dl_070127D0);
	EXPORT_DL(lll_seg7_dl_07012AE8);
	EXPORT_DL(lll_seg7_dl_07012C20);
	EXPORT_DL(lll_seg7_dl_07012C78);
	EXPORT_DL(lll_seg7_dl_07012CC0);
	EXPORT_DL(lll_seg7_dl_07012F60);
	EXPORT_DL(lll_seg7_dl_070130A8);
	EXPORT_DL(lll_seg7_dl_070132D8);
	EXPORT_DL(lll_seg7_dl_07013320);
	EXPORT_DL(lll_seg7_dl_07013378);
	EXPORT_DL(lll_seg7_dl_07013430);
	EXPORT_DL(lll_seg7_dl_07013488);
	EXPORT_DL(lll_seg7_dl_070134E0);
	EXPORT_DL(lll_seg7_dl_07013738);
	EXPORT_DL(lll_seg7_dl_070137C0);
	EXPORT_DL(lll_seg7_dl_070138B0);
	EXPORT_DL(lll_seg7_dl_070138F8);
	EXPORT_DL(lll_seg7_dl_07013C08);
	EXPORT_DL(lll_seg7_dl_07013C70);
	EXPORT_DL(lll_seg7_dl_07013D28);
	EXPORT_DL(lll_seg7_dl_070144F8);
	EXPORT_DL(lll_seg7_dl_070146B8);
	EXPORT_DL(lll_seg7_dl_07014788);
	EXPORT_DL(lll_seg7_dl_07014AB8);
	EXPORT_DL(lll_seg7_dl_07014B80);
	EXPORT_DL(lll_seg7_dl_07014BD8);
	EXPORT_DL(lll_seg7_dl_07015208);
	EXPORT_DL(lll_seg7_dl_07015270);
	EXPORT_DL(lll_seg7_dl_07015328);
	EXPORT_DL(lll_seg7_dl_070153F0);
	EXPORT_DL(lll_seg7_dl_07015458);
	EXPORT_DL(lll_seg7_dl_07015B38);
	EXPORT_DL(lll_seg7_dl_07015C20);
	EXPORT_DL(lll_seg7_dl_07015C88);
	EXPORT_DL(lll_seg7_dl_07015D18);
	EXPORT_DL(lll_seg7_dl_07015E20);
	EXPORT_DL(lll_seg7_dl_07016130);
	EXPORT_DL(lll_seg7_dl_07016198);
	EXPORT_DL(lll_seg7_dl_07016250);
	EXPORT_DL(lll_seg7_dl_070164E0);
	EXPORT_DL(lll_seg7_dl_07016538);
	EXPORT_DL(lll_seg7_dl_070165C8);
	EXPORT_DL(lll_seg7_dl_07016930);
	EXPORT_DL(lll_seg7_dl_07016A30);
	EXPORT_DL(lll_seg7_dl_07016A78);
	EXPORT_DL(lll_seg7_dl_07016B00);
	EXPORT_DL(lll_seg7_dl_07017250);
	EXPORT_DL(lll_seg7_dl_070172B8);
	EXPORT_DL(lll_seg7_dl_07017358);
	EXPORT_DL(lll_seg7_dl_070174E0);
	EXPORT_DL(lll_seg7_dl_070177B0);
	EXPORT_DL(lll_seg7_dl_07017818);
	EXPORT_DL(lll_seg7_dl_070178A8);
	EXPORT_DL(lll_seg7_dl_07017A78);
	EXPORT_DL(lll_seg7_dl_07017AE0);
	EXPORT_DL(lll_seg7_dl_07017B18);
	EXPORT_DL(lll_seg7_dl_07017B50);
	EXPORT_DL(lll_seg7_dl_07017E68);
	EXPORT_DL(lll_seg7_dl_07017F40);
	EXPORT_DL(lll_seg7_dl_07018280);
	EXPORT_DL(lll_seg7_dl_07018380);
	EXPORT_DL(lll_seg7_dl_07018570);
	EXPORT_DL(lll_seg7_dl_070185C8);
	EXPORT_DL(lll_seg7_dl_07018600);
	EXPORT_DL(lll_seg7_dl_07018638);
	EXPORT_DL(lll_seg7_dl_07018680);
	EXPORT_DL(lll_seg7_dl_07018968);
	EXPORT_DL(lll_seg7_dl_07018A30);
	EXPORT_DL(lll_seg7_dl_07018BE0);
	EXPORT_DL(lll_seg7_dl_07018C38);
	EXPORT_DL(lll_seg7_dl_07018C90);
	EXPORT_DL(lll_seg7_dl_07018E48);
	EXPORT_DL(lll_seg7_dl_07018EA0);
	EXPORT_DL(lll_seg7_dl_07018EF8);
	EXPORT_DL(lll_seg7_dl_070190B0);
	EXPORT_DL(lll_seg7_dl_070190F8);
	EXPORT_DL(lll_seg7_dl_07019160);
	EXPORT_DL(lll_seg7_dl_07019330);
	EXPORT_DL(lll_seg7_dl_07019378);
	EXPORT_DL(lll_seg7_dl_070193E0);
	EXPORT_DL(lll_seg7_dl_07019870);
	EXPORT_DL(lll_seg7_dl_07019908);
	EXPORT_DL(lll_seg7_dl_07019A08);
	EXPORT_DL(lll_seg7_dl_07019B78);
	EXPORT_DL(lll_seg7_dl_07019BD0);
	EXPORT_DL(lll_seg7_dl_07019C08);
	EXPORT_DL(lll_seg7_dl_07019F20);
	EXPORT_DL(lll_seg7_dl_0701A010);
	EXPORT_DL(lll_seg7_dl_0701A160);
	EXPORT_DL(lll_seg7_dl_0701A1A8);
	EXPORT_DL(lll_seg7_dl_0701A1F0);
	EXPORT_DL(lll_seg7_dl_0701A2F0);
	EXPORT_DL(lll_seg7_dl_0701A338);
	EXPORT_DL(lll_seg7_dl_0701A388);
	EXPORT_DL(lll_seg7_dl_0701A3B8);
	EXPORT_DL(lll_seg7_dl_0701A3E8);
	EXPORT_DL(lll_seg7_dl_0701A418);
	EXPORT_DL(lll_seg7_dl_0701A448);
	EXPORT_DL(lll_seg7_dl_0701A478);
	EXPORT_DL(lll_seg7_dl_0701A4A8);
	EXPORT_DL(lll_seg7_dl_0701A4D8);
	EXPORT_DL(lll_seg7_dl_0701A508);
	EXPORT_DL(lll_seg7_dl_0701A538);
	EXPORT_DL(lll_seg7_dl_0701A568);
	EXPORT_DL(lll_seg7_dl_0701A598);
	EXPORT_DL(lll_seg7_dl_0701A5C8);
	EXPORT_DL(lll_seg7_dl_0701A5F8);
	EXPORT_DL(lll_seg7_dl_0701A628);
	EXPORT_DL(lll_seg7_dl_0701A7E8);
	EXPORT_DL(lll_seg7_dl_0701A878);
	EXPORT_DL(lll_seg7_dl_0701AC20);
	EXPORT_DL(lll_seg7_dl_0701ACE8);
	EXPORT_DL(lll_seg7_dl_0701AD70);
	EXPORT_DL(lll_seg7_dl_07021E20);
	EXPORT_DL(lll_seg7_dl_070223C0);
	EXPORT_DL(lll_seg7_dl_070229B8);
	EXPORT_DL(lll_seg7_dl_07022AE0);
	EXPORT_DL(lll_seg7_dl_07022DF0);
	EXPORT_DL(lll_seg7_dl_070235C8);
	EXPORT_DL(lll_seg7_dl_07024630);
	EXPORT_DL(lll_seg7_dl_070247C8);
	EXPORT_DL(lll_seg7_dl_07024B08);
	EXPORT_DL(lll_seg7_dl_07024B40);
	EXPORT_DL(lll_seg7_dl_07024C18);
	EXPORT_DL(lll_seg7_dl_070253D8);
	EXPORT_DL(lll_seg7_dl_070255D8);
	EXPORT_DL(lll_seg7_dl_070258F8);
	EXPORT_DL(lll_seg7_dl_07025970);
	EXPORT_DL(lll_seg7_dl_07025A48);
	EXPORT_DL(lll_seg7_dl_07025B80);
	EXPORT_DL(lll_seg7_dl_07025BD8);
	EXPORT_DL(lll_seg7_dl_07025E10);
	EXPORT_DL(lll_seg7_dl_07025E78);
	EXPORT_DL(lll_seg7_dl_07025EC0);
	EXPORT_DL(lll_dl_lava_floor);
	EXPORT_DL(lll_dl_lavafall_volcano);
}
