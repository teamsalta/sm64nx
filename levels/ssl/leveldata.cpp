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
// LEVEL DATA
#include "levels/ssl/texture.inc.c"
// This is an oddity, but it is assumed this because of the likelyhood SSL's
// course used to be smaller in a prototype build and that moving areas around
// caused this discrepency. (See the 3 Star Course theory)
#include "levels/ssl/areas/2/4/model.inc.c"
#include "levels/ssl/areas/1/1/model.inc.c"
#include "levels/ssl/areas/1/2/model.inc.c"
#include "levels/ssl/areas/1/3/model.inc.c"
#include "levels/ssl/areas/1/4/model.inc.c"
#include "levels/ssl/pyramid_top/model.inc.c"
#include "levels/ssl/tox_box/model.inc.c"
#include "levels/ssl/areas/1/collision.inc.c"
#include "levels/ssl/areas/1/macro.inc.c"
#include "levels/ssl/pyramid_top/collision.inc.c"
#include "levels/ssl/tox_box/collision.inc.c"
#include "levels/ssl/areas/1/movtext.inc.c"
#include "levels/ssl/areas/2/1/model.inc.c"
#include "levels/ssl/areas/2/2/model.inc.c"
#include "levels/ssl/areas/2/3/model.inc.c"
#include "levels/ssl/areas/3/1/model.inc.c"
#include "levels/ssl/areas/3/2/model.inc.c"
#include "levels/ssl/grindel/model.inc.c"
#include "levels/ssl/spindel/model.inc.c"
#include "levels/ssl/moving_pyramid_wall/model.inc.c"
#include "levels/ssl/pyramid_elevator/model.inc.c"
#include "levels/ssl/eyerok_col/model.inc.c" // Blank file
#include "levels/ssl/areas/2/collision.inc.c"
#include "levels/ssl/areas/3/collision.inc.c"
#include "levels/ssl/areas/2/macro.inc.c"
#include "levels/ssl/areas/3/macro.inc.c"
#include "levels/ssl/grindel/collision.inc.c"
#include "levels/ssl/spindel/collision.inc.c"
#include "levels/ssl/moving_pyramid_wall/collision.inc.c"
#include "levels/ssl/pyramid_elevator/collision.inc.c"
#include "levels/ssl/eyerok_col/collision.inc.c"
#include "levels/ssl/areas/2/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(ssl_dl_quicksand_pit_begin);
	EXPORT_DL(ssl_dl_quicksand_pit_end);
	EXPORT_DL(ssl_dl_pyramid_quicksand_pit_begin);
	EXPORT_DL(ssl_dl_pyramid_quicksand_pit_end);
	EXPORT_DL(ssl_dl_quicksand_pit);
	EXPORT_DL(ssl_dl_pyramid_quicksand_pit_static);
	EXPORT_DL(ssl_seg7_dl_07008C40);
	EXPORT_DL(ssl_seg7_dl_07008DE0);
	EXPORT_DL(ssl_seg7_dl_07009170);
	EXPORT_DL(ssl_seg7_dl_07009410);
	EXPORT_DL(ssl_seg7_dl_07009950);
	EXPORT_DL(ssl_seg7_dl_07009A40);
	EXPORT_DL(ssl_seg7_dl_07009C60);
	EXPORT_DL(ssl_seg7_dl_07009E90);
	EXPORT_DL(ssl_seg7_dl_07009F48);
	EXPORT_DL(ssl_seg7_dl_0700B2E8);
	EXPORT_DL(ssl_seg7_dl_0700BA78);
	EXPORT_DL(ssl_seg7_dl_0700BBB0);
	EXPORT_DL(ssl_seg7_dl_0700BC18);
	EXPORT_DL(ssl_seg7_dl_0700BCC8);
	EXPORT_DL(ssl_seg7_dl_0700BD00);
	EXPORT_DL(ssl_seg7_dl_0700BE88);
	EXPORT_DL(ssl_seg7_dl_0700BEE0);
	EXPORT_DL(ssl_seg7_dl_0700BF18);
	EXPORT_DL(ssl_seg7_dl_0700FB48);
	EXPORT_DL(ssl_seg7_dl_0700FBD8);
	EXPORT_DL(ssl_seg7_dl_0700FC40);
	EXPORT_DL(ssl_seg7_dl_0700FCA8);
	EXPORT_DL(ssl_seg7_dl_0700FCE0);
	EXPORT_DL(ssl_dl_quicksand_begin);
	EXPORT_DL(ssl_dl_quicksand_end);
	EXPORT_DL(ssl_dl_pyramid_quicksand);
	EXPORT_DL(ssl_dl_pyramid_corners_quicksand);
	EXPORT_DL(ssl_dl_sides_quicksand);
	EXPORT_DL(ssl_seg7_dl_0701C198);
	EXPORT_DL(ssl_seg7_dl_0701CFA8);
	EXPORT_DL(ssl_seg7_dl_0701D060);
	EXPORT_DL(ssl_seg7_dl_0701E980);
	EXPORT_DL(ssl_seg7_dl_0701EAD8);
	EXPORT_DL(ssl_seg7_dl_0701ECB0);
	EXPORT_DL(ssl_seg7_dl_0701EE80);
	EXPORT_DL(ssl_seg7_dl_0701F700);
	EXPORT_DL(ssl_seg7_dl_0701F920);
	EXPORT_DL(ssl_seg7_dl_0701FC18);
	EXPORT_DL(ssl_seg7_dl_0701FCE0);
	EXPORT_DL(ssl_seg7_dl_07021290);
	EXPORT_DL(ssl_seg7_dl_070214E8);
	EXPORT_DL(ssl_seg7_dl_07021638);
	EXPORT_DL(ssl_seg7_dl_07021870);
	EXPORT_DL(ssl_seg7_dl_07021910);
	EXPORT_DL(ssl_seg7_dl_07021968);
	EXPORT_DL(ssl_seg7_dl_070219D0);
	EXPORT_DL(ssl_seg7_dl_07021A08);
	EXPORT_DL(ssl_seg7_dl_07021D00);
	EXPORT_DL(ssl_seg7_dl_07021D68);
	EXPORT_DL(ssl_seg7_dl_07021DE8);
	EXPORT_DL(ssl_seg7_dl_07021FE8);
	EXPORT_DL(ssl_seg7_dl_07022040);
	EXPORT_DL(ssl_seg7_dl_070220A8);
	EXPORT_DL(ssl_seg7_dl_070221B0);
	EXPORT_DL(ssl_seg7_dl_070221E8);
	EXPORT_DL(ssl_seg7_dl_07022728);
	EXPORT_DL(ssl_seg7_dl_070227F0);
	EXPORT_DL(ssl_seg7_dl_070228A8);
	EXPORT_DL(ssl_seg7_dl_070229B0);
	EXPORT_DL(ssl_seg7_dl_070229E8);
	EXPORT_DL(ssl_seg7_dl_07022C38);
	EXPORT_DL(ssl_seg7_dl_07022CB0);
	EXPORT_DL(ssl_seg7_dl_07022CF8);
	EXPORT_DL(ssl_seg7_dl_070231D0);
	EXPORT_DL(ssl_seg7_dl_07023268);
	EXPORT_DL(ssl_seg7_dl_070233A8);
	EXPORT_DL(ssl_seg7_dl_07023558);
	EXPORT_DL(ssl_seg7_dl_070235C0);
	EXPORT_DL(ssl_dl_pyramid_sand_pathway_floor_begin);
	EXPORT_DL(ssl_dl_pyramid_sand_pathway_floor_end);
	EXPORT_DL(ssl_dl_pyramid_sand_pathway_begin);
	EXPORT_DL(ssl_dl_pyramid_sand_pathway_end);
	EXPORT_DL(ssl_dl_pyramid_sand_pathway_front_end);
	EXPORT_DL(ssl_dl_pyramid_sand_pathway_side_end);
}
