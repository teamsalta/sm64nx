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
#include "levels/rr/texture.inc.c"
#include "levels/rr/areas/1/1/model.inc.c"
#include "levels/rr/areas/1/2/model.inc.c"
#include "levels/rr/areas/1/3/model.inc.c"
#include "levels/rr/areas/1/4/model.inc.c"
#include "levels/rr/areas/1/5/model.inc.c"
#include "levels/rr/areas/1/6/model.inc.c"
#include "levels/rr/areas/1/7/model.inc.c"
#include "levels/rr/areas/1/8/model.inc.c"
#include "levels/rr/areas/1/9/model.inc.c"
#include "levels/rr/areas/1/10/model.inc.c"
#include "levels/rr/areas/1/11/1.inc.c"
#include "levels/rr/areas/1/11/2.inc.c"
#include "levels/rr/areas/1/12/model.inc.c"
#include "levels/rr/areas/1/13/model.inc.c"
#include "levels/rr/areas/1/14/model.inc.c"
#include "levels/rr/areas/1/15/model.inc.c"
#include "levels/rr/areas/1/16/model.inc.c"
#include "levels/rr/areas/1/17/model.inc.c"
#include "levels/rr/areas/1/18/model.inc.c"
#include "levels/rr/areas/1/19/model.inc.c"
#include "levels/rr/areas/1/20/model.inc.c"
#include "levels/rr/areas/1/21/model.inc.c"
#include "levels/rr/flying_carpet/model.inc.c"
#include "levels/rr/swinging_platform/model.inc.c"
#include "levels/rr/rotating_bridge_platform/model.inc.c"
#include "levels/rr/cruiser_wing/model.inc.c"
#include "levels/rr/octagonal_platform/model.inc.c"
#include "levels/rr/sliding_platform/model.inc.c"
#include "levels/rr/pyramid_platform/model.inc.c" // Unused
#include "levels/rr/elevator_platform/model.inc.c"
#include "levels/rr/seesaw_platform/model.inc.c"
#include "levels/rr/donut_block/1.inc.c"
#include "levels/rr/donut_block/2.inc.c"
#include "levels/rr/l_platform/model.inc.c" // also unused
#include "levels/rr/tricky_triangles_1/model.inc.c"
#include "levels/rr/tricky_triangles_2/model.inc.c"
#include "levels/rr/tricky_triangles_3/model.inc.c"
#include "levels/rr/tricky_triangles_4/model.inc.c"
#include "levels/rr/tricky_triangles_5/model.inc.c"
#include "levels/rr/flying_carpet/collision.inc.c"
#include "levels/rr/swinging_platform/collision.inc.c"
#include "levels/rr/rotating_bridge_platform/collision.inc.c"
#include "levels/rr/octagonal_platform/collision.inc.c"
#include "levels/rr/sliding_platform/collision.inc.c"
#include "levels/rr/pyramid_platform/collision.inc.c"
#include "levels/rr/elevator_platform/collision.inc.c"
#include "levels/rr/seesaw_platform/collision.inc.c"
#include "levels/rr/donut_block/collision.inc.c"
#include "levels/rr/l_platform/collision.inc.c"
#include "levels/rr/tricky_triangles_1/collision.inc.c"
#include "levels/rr/tricky_triangles_2/collision.inc.c"
#include "levels/rr/tricky_triangles_3/collision.inc.c"
#include "levels/rr/tricky_triangles_4/collision.inc.c"
#include "levels/rr/tricky_triangles_5/collision.inc.c"
#include "levels/rr/areas/1/collision.inc.c"
#include "levels/rr/areas/1/macro.inc.c"
#include "levels/rr/areas/1/trajectory.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(dl_cruiser_metal_holes_model);
	EXPORT_DL(dl_cruiser_metal_holes);
	EXPORT_DL(rr_seg7_dl_07002A18);
	EXPORT_DL(rr_seg7_dl_07002BC8);
	EXPORT_DL(rr_seg7_dl_07002CC8);
	EXPORT_DL(rr_seg7_dl_07002E00);
	EXPORT_DL(rr_seg7_dl_07002E38);
	EXPORT_DL(rr_seg7_dl_07002E80);
	EXPORT_DL(rr_seg7_dl_07004468);
	EXPORT_DL(rr_seg7_dl_07004950);
	EXPORT_DL(rr_seg7_dl_07004A98);
	EXPORT_DL(rr_seg7_dl_070057F0);
	EXPORT_DL(rr_seg7_dl_07005A48);
	EXPORT_DL(rr_seg7_dl_07005C18);
	EXPORT_DL(rr_seg7_dl_07005C80);
	EXPORT_DL(rr_seg7_dl_070076B0);
	EXPORT_DL(rr_seg7_dl_07007868);
	EXPORT_DL(rr_seg7_dl_070079B0);
	EXPORT_DL(rr_seg7_dl_07007CB0);
	EXPORT_DL(rr_seg7_dl_07007D88);
	EXPORT_DL(rr_seg7_dl_07007E60);
	EXPORT_DL(rr_seg7_dl_07008170);
	EXPORT_DL(rr_seg7_dl_07008258);
	EXPORT_DL(rr_seg7_dl_07008B58);
	EXPORT_DL(rr_seg7_dl_07008D28);
	EXPORT_DL(rr_seg7_dl_07008E20);
	EXPORT_DL(rr_seg7_dl_0700B138);
	EXPORT_DL(rr_seg7_dl_0700BB48);
	EXPORT_DL(rr_seg7_dl_0700C698);
	EXPORT_DL(rr_seg7_dl_0700C820);
	EXPORT_DL(rr_seg7_dl_0700C9B8);
	EXPORT_DL(rr_seg7_dl_0700CA38);
	EXPORT_DL(rr_seg7_dl_0700D768);
	EXPORT_DL(rr_seg7_dl_0700DAD8);
	EXPORT_DL(rr_seg7_dl_0700DBA8);
	EXPORT_DL(rr_seg7_dl_0700DBD8);
	EXPORT_DL(rr_seg7_dl_0700DDF8);
	EXPORT_DL(rr_seg7_dl_0700DE88);
	EXPORT_DL(rr_seg7_dl_0700E0A8);
	EXPORT_DL(rr_seg7_dl_0700E110);
	EXPORT_DL(rr_seg7_dl_0700E178);
	EXPORT_DL(rr_seg7_dl_0700E6C8);
	EXPORT_DL(rr_seg7_dl_0700E830);
	EXPORT_DL(rr_seg7_dl_07011920);
	EXPORT_DL(rr_seg7_dl_07011978);
	EXPORT_DL(rr_seg7_dl_07012128);
	EXPORT_DL(rr_seg7_dl_07012418);
	EXPORT_DL(rr_seg7_dl_070124A8);
	EXPORT_DL(rr_seg7_dl_07012758);
	EXPORT_DL(rr_seg7_dl_07013E68);
	EXPORT_DL(rr_seg7_dl_070142C0);
	EXPORT_DL(rr_seg7_dl_07014350);
	EXPORT_DL(rr_seg7_dl_07014490);
	EXPORT_DL(rr_seg7_dl_07014508);
	EXPORT_DL(rr_seg7_dl_07014CD0);
	EXPORT_DL(rr_seg7_dl_07014D80);
	EXPORT_DL(rr_seg7_dl_07014F30);
	EXPORT_DL(rr_seg7_dl_07015508);
	EXPORT_DL(rr_seg7_dl_07015570);
	EXPORT_DL(rr_seg7_dl_070156B8);
	EXPORT_DL(rr_seg7_dl_07015B28);
	EXPORT_DL(rr_seg7_dl_07015C68);
	EXPORT_DL(rr_seg7_dl_07016FD8);
	EXPORT_DL(rr_seg7_dl_07017530);
	EXPORT_DL(rr_seg7_dl_07018650);
	EXPORT_DL(rr_seg7_dl_07018938);
	EXPORT_DL(rr_seg7_dl_07018980);
	EXPORT_DL(rr_seg7_dl_07018B58);
	EXPORT_DL(rr_seg7_dl_07019038);
	EXPORT_DL(rr_seg7_dl_070190A0);
	EXPORT_DL(rr_seg7_dl_070191A0);
	EXPORT_DL(dl_flying_carpet_begin);
	EXPORT_DL(dl_flying_carpet_model_half);
	EXPORT_DL(dl_flying_carpet_end);
	EXPORT_DL(rr_seg7_dl_070198F0);
	EXPORT_DL(rr_seg7_dl_07019A10);
	EXPORT_DL(rr_seg7_dl_07019A58);
	EXPORT_DL(rr_seg7_dl_07019AC0);
	EXPORT_DL(rr_seg7_dl_07019AE0);
	EXPORT_DL(rr_seg7_dl_0701A2C0);
	EXPORT_DL(rr_seg7_dl_0701A3A8);
	EXPORT_DL(rr_seg7_dl_0701A3E0);
	EXPORT_DL(rr_seg7_dl_0701A4E8);
	EXPORT_DL(rr_seg7_dl_0701A568);
	EXPORT_DL(rr_seg7_dl_0701A648);
	EXPORT_DL(rr_seg7_dl_0701A680);
	EXPORT_DL(rr_seg7_dl_0701AA48);
	EXPORT_DL(rr_seg7_dl_0701AB78);
	EXPORT_DL(rr_seg7_dl_0701AD68);
	EXPORT_DL(rr_seg7_dl_0701ADF8);
	EXPORT_DL(rr_seg7_dl_0701B190);
	EXPORT_DL(rr_seg7_dl_0701B220);
	EXPORT_DL(rr_seg7_dl_0701B430);
	EXPORT_DL(rr_seg7_dl_0701B4C0);
	EXPORT_DL(rr_seg7_dl_0701B770);
	EXPORT_DL(rr_seg7_dl_0701B848);
	EXPORT_DL(rr_seg7_dl_0701B938);
	EXPORT_DL(rr_seg7_dl_0701B980);
	EXPORT_DL(rr_seg7_dl_0701DDE8);
	EXPORT_DL(rr_seg7_dl_0701E3F8);
	EXPORT_DL(rr_seg7_dl_0701E670);
	EXPORT_DL(rr_seg7_dl_07020870);
	EXPORT_DL(rr_seg7_dl_07020E80);
	EXPORT_DL(rr_seg7_dl_07021100);
	EXPORT_DL(rr_seg7_dl_07023300);
	EXPORT_DL(rr_seg7_dl_07023910);
	EXPORT_DL(rr_seg7_dl_07023B90);
	EXPORT_DL(rr_seg7_dl_07025DA0);
	EXPORT_DL(rr_seg7_dl_070263B0);
	EXPORT_DL(rr_seg7_dl_07026630);
	EXPORT_DL(rr_seg7_dl_07028720);
	EXPORT_DL(rr_seg7_dl_07028D30);
	EXPORT_DL(rr_seg7_dl_07028FA8);
}
