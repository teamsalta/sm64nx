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
#include "levels/hmc/texture.inc.c"
#include "levels/hmc/areas/1/1/model.inc.c"
#include "levels/hmc/areas/1/2/model.inc.c"
#include "levels/hmc/areas/1/3/model.inc.c"
#include "levels/hmc/areas/1/4/model.inc.c"
#include "levels/hmc/areas/1/5/model.inc.c"
#include "levels/hmc/areas/1/6/model.inc.c"
#include "levels/hmc/areas/1/7/model.inc.c"
#include "levels/hmc/areas/1/8/model.inc.c"
#include "levels/hmc/areas/1/9/model.inc.c"
#include "levels/hmc/areas/1/10/model.inc.c"
#include "levels/hmc/areas/1/11/model.inc.c"
#include "levels/hmc/areas/1/12/model.inc.c"
#include "levels/hmc/areas/1/13/model.inc.c"
#include "levels/hmc/areas/1/14/model.inc.c"
#include "levels/hmc/areas/1/15/model.inc.c"
#include "levels/hmc/areas/1/16/model.inc.c"
#include "levels/hmc/areas/1/17/model.inc.c"
#include "levels/hmc/areas/1/18/model.inc.c"
#include "levels/hmc/areas/1/19/model.inc.c"
#include "levels/hmc/areas/1/20/model.inc.c"
#include "levels/hmc/areas/1/21/model.inc.c"
#include "levels/hmc/areas/1/22/model.inc.c"
#include "levels/hmc/areas/1/23/model.inc.c"
#include "levels/hmc/areas/1/24/model.inc.c"
#include "levels/hmc/areas/1/25/model.inc.c"
#include "levels/hmc/areas/1/26/model.inc.c"
#include "levels/hmc/areas/1/27/model.inc.c"
#include "levels/hmc/areas/1/28/model.inc.c"
#include "levels/hmc/areas/1/grill_door/model.inc.c"
#include "levels/hmc/areas/1/29/model.inc.c"
#include "levels/hmc/areas/1/30/model.inc.c"
#include "levels/hmc/areas/1/31/model.inc.c"
#include "levels/hmc/areas/1/32/model.inc.c"
#include "levels/hmc/elevator_platform/model.inc.c"
#include "levels/hmc/arrow_platform/model.inc.c"
#include "levels/hmc/arrow_platform_button/model.inc.c"
#include "levels/hmc/rolling_rock/model.inc.c"
#include "levels/hmc/rolling_rock_fragment_1/model.inc.c"
#include "levels/hmc/rolling_rock_fragment_2/model.inc.c"
#include "levels/hmc/areas/1/painting.inc.c"
#include "levels/hmc/areas/1/collision.inc.c"
#include "levels/hmc/areas/1/macro.inc.c"
#include "levels/hmc/areas/1/room.inc.c"
#include "levels/hmc/elevator_platform/collision.inc.c"
#include "levels/hmc/areas/1/grill_door/collision.inc.c" // This is odd, but the model data for this is in the middle of area 1. I have to conclude it is an area local object, despite the fact it has collision. Let's hope this is the only oddity...
#include "levels/hmc/arrow_platform/collision.inc.c"
#include "levels/hmc/arrow_platform_button/collision.inc.c"
#include "levels/hmc/areas/1/trajectory.inc.c"
#include "levels/hmc/areas/1/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(hmc_seg7_dl_07006FF8);
	EXPORT_DL(hmc_seg7_dl_07007080);
	EXPORT_DL(hmc_seg7_dl_07007278);
	EXPORT_DL(hmc_seg7_dl_07007490);
	EXPORT_DL(hmc_seg7_dl_070074C8);
	EXPORT_DL(hmc_seg7_dl_07007500);
	EXPORT_DL(hmc_seg7_dl_07007630);
	EXPORT_DL(hmc_seg7_dl_070078B0);
	EXPORT_DL(hmc_seg7_dl_07007AE8);
	EXPORT_DL(hmc_seg7_dl_07007B50);
	EXPORT_DL(hmc_seg7_dl_07007FC8);
	EXPORT_DL(hmc_seg7_dl_070080E8);
	EXPORT_DL(hmc_seg7_dl_07008F98);
	EXPORT_DL(hmc_seg7_dl_070093F0);
	EXPORT_DL(hmc_seg7_dl_0700CF78);
	EXPORT_DL(hmc_seg7_dl_0700D1C8);
	EXPORT_DL(hmc_seg7_dl_0700D3D8);
	EXPORT_DL(hmc_seg7_dl_0700D8D8);
	EXPORT_DL(hmc_seg7_dl_0700D9A8);
	EXPORT_DL(hmc_seg7_dl_0700DD30);
	EXPORT_DL(hmc_seg7_dl_0700DED8);
	EXPORT_DL(hmc_seg7_dl_0700DF10);
	EXPORT_DL(hmc_seg7_dl_0700DF48);
	EXPORT_DL(hmc_seg7_dl_0700E330);
	EXPORT_DL(hmc_seg7_dl_0700E428);
	EXPORT_DL(hmc_seg7_dl_0700E448);
	EXPORT_DL(hmc_seg7_dl_0700ECB0);
	EXPORT_DL(hmc_seg7_dl_0700EE38);
	EXPORT_DL(hmc_seg7_dl_0700EF00);
	EXPORT_DL(hmc_seg7_dl_0700F2F8);
	EXPORT_DL(hmc_seg7_dl_0700F3E8);
	EXPORT_DL(hmc_seg7_dl_0700F8B8);
	EXPORT_DL(hmc_seg7_dl_0700FA40);
	EXPORT_DL(hmc_seg7_dl_0700FDF0);
	EXPORT_DL(hmc_seg7_dl_0700FEF0);
	EXPORT_DL(hmc_seg7_dl_07010008);
	EXPORT_DL(hmc_seg7_dl_07010070);
	EXPORT_DL(hmc_seg7_dl_07012C78);
	EXPORT_DL(hmc_seg7_dl_07013080);
	EXPORT_DL(hmc_seg7_dl_07013168);
	EXPORT_DL(hmc_seg7_dl_07013840);
	EXPORT_DL(hmc_seg7_dl_070138E0);
	EXPORT_DL(hmc_seg7_dl_07013A78);
	EXPORT_DL(hmc_seg7_dl_07013B48);
	EXPORT_DL(hmc_seg7_dl_07013BC8);
	EXPORT_DL(hmc_seg7_dl_07013C78);
	EXPORT_DL(hmc_seg7_dl_07013CA8);
	EXPORT_DL(hmc_seg7_dl_07013E38);
	EXPORT_DL(hmc_seg7_dl_07013E80);
	EXPORT_DL(hmc_seg7_dl_07014218);
	EXPORT_DL(hmc_seg7_dl_07014300);
	EXPORT_DL(hmc_seg7_dl_07014950);
	EXPORT_DL(hmc_seg7_dl_07014B08);
	EXPORT_DL(hmc_seg7_dl_07014BC8);
	EXPORT_DL(hmc_seg7_dl_07014C00);
	EXPORT_DL(hmc_seg7_dl_07014DD0);
	EXPORT_DL(hmc_seg7_dl_07014E48);
	EXPORT_DL(hmc_seg7_dl_07016918);
	EXPORT_DL(hmc_seg7_dl_07016A90);
	EXPORT_DL(hmc_seg7_dl_07016BA8);
	EXPORT_DL(hmc_seg7_dl_07016C60);
	EXPORT_DL(hmc_seg7_dl_07016E40);
	EXPORT_DL(hmc_seg7_dl_07017338);
	EXPORT_DL(hmc_seg7_dl_07017370);
	EXPORT_DL(hmc_seg7_dl_070173A8);
	EXPORT_DL(hmc_seg7_dl_07017AA0);
	EXPORT_DL(hmc_seg7_dl_07017C08);
	EXPORT_DL(hmc_seg7_dl_07017C98);
	EXPORT_DL(hmc_seg7_dl_070180F0);
	EXPORT_DL(hmc_seg7_dl_07018200);
	EXPORT_DL(hmc_seg7_dl_07018E80);
	EXPORT_DL(hmc_seg7_dl_07019248);
	EXPORT_DL(hmc_seg7_dl_07019328);
	EXPORT_DL(hmc_seg7_dl_07019368);
	EXPORT_DL(hmc_seg7_dl_07019D18);
	EXPORT_DL(hmc_seg7_dl_07019DC8);
	EXPORT_DL(hmc_seg7_dl_07019F70);
	EXPORT_DL(hmc_seg7_dl_0701A080);
	EXPORT_DL(hmc_seg7_dl_0701A340);
	EXPORT_DL(hmc_seg7_dl_0701A400);
	EXPORT_DL(hmc_seg7_dl_0701D5A0);
	EXPORT_DL(hmc_seg7_dl_0701D708);
	EXPORT_DL(hmc_seg7_dl_0701D8B0);
	EXPORT_DL(hmc_seg7_dl_0701E228);
	EXPORT_DL(hmc_seg7_dl_0701E260);
	EXPORT_DL(hmc_seg7_dl_0701E370);
	EXPORT_DL(hmc_seg7_dl_0701E528);
	EXPORT_DL(hmc_seg7_dl_0701E688);
	EXPORT_DL(hmc_seg7_dl_0701E6D0);
	EXPORT_DL(hmc_seg7_dl_0701E718);
	EXPORT_DL(hmc_seg7_dl_0701E820);
	EXPORT_DL(hmc_seg7_dl_0701EF78);
	EXPORT_DL(hmc_seg7_dl_0701F0D0);
	EXPORT_DL(hmc_seg7_dl_0701F1B0);
	EXPORT_DL(hmc_seg7_dl_0701F558);
	EXPORT_DL(hmc_seg7_dl_0701F690);
	EXPORT_DL(hmc_seg7_dl_0701F7C0);
	EXPORT_DL(hmc_seg7_dl_0701F818);
	EXPORT_DL(hmc_seg7_dl_0701FC28);
	EXPORT_DL(hmc_seg7_dl_0701FD58);
	EXPORT_DL(hmc_seg7_dl_0701FF68);
	EXPORT_DL(hmc_seg7_dl_0701FFF8);
	EXPORT_DL(hmc_seg7_dl_07020B18);
	EXPORT_DL(hmc_seg7_dl_07020BD8);
	EXPORT_DL(hmc_seg7_dl_07020C60);
	EXPORT_DL(hmc_seg7_dl_07020D50);
	EXPORT_DL(hmc_seg7_dl_07020EA0);
	EXPORT_DL(hmc_seg7_dl_07020F90);
	EXPORT_DL(hmc_seg7_dl_07020FD0);
	EXPORT_DL(hmc_seg7_dl_070215B8);
	EXPORT_DL(hmc_seg7_dl_07021630);
	EXPORT_DL(hmc_seg7_dl_07021760);
	EXPORT_DL(hmc_seg7_dl_07021AC8);
	EXPORT_DL(hmc_seg7_dl_07021BA0);
	EXPORT_DL(hmc_seg7_dl_070225B0);
	EXPORT_DL(hmc_seg7_dl_070228A0);
	EXPORT_DL(hmc_seg7_dl_07022A08);
	EXPORT_DL(hmc_seg7_dl_07022AA0);
	EXPORT_DL(hmc_seg7_dl_07022CE0);
	EXPORT_DL(hmc_seg7_dl_07022D58);
	EXPORT_DL(hmc_seg7_dl_07022DA0);
	EXPORT_DL(hmc_seg7_dl_07022FE8);
	EXPORT_DL(hmc_seg7_dl_07023030);
	EXPORT_DL(hmc_seg7_dl_07023090);
	EXPORT_DL(hmc_seg7_dl_070238D8);
	EXPORT_DL(hmc_seg7_dl_07023BC8);
	EXPORT_DL(hmc_seg7_dl_07023D48);
	EXPORT_DL(hmc_seg7_dl_07023E10);
	EXPORT_DL(hmc_seg7_dl_07024080);
	EXPORT_DL(hmc_seg7_dl_07024110);
	EXPORT_DL(hmc_seg7_dl_07024268);
	EXPORT_DL(hmc_seg7_dl_070242A0);
	EXPORT_DL(hmc_seg7_painting_dl_070242D0);
	EXPORT_DL(hmc_seg7_painting_dl_070254E0);
}
