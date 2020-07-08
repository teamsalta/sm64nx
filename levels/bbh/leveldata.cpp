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
#include "levels/bbh/texture.inc.c"
#include "levels/bbh/areas/1/1/model.inc.c"
#include "levels/bbh/areas/1/2/model.inc.c"
#include "levels/bbh/areas/1/3/model.inc.c"
#include "levels/bbh/areas/1/4/model.inc.c"
#include "levels/bbh/areas/1/5/model.inc.c"
#include "levels/bbh/areas/1/6/model.inc.c"
#include "levels/bbh/areas/1/7/model.inc.c"
#include "levels/bbh/areas/1/8/model.inc.c"
#include "levels/bbh/areas/1/9/model.inc.c"
#include "levels/bbh/areas/1/10/model.inc.c"
#include "levels/bbh/areas/1/11/model.inc.c"
#include "levels/bbh/areas/1/12/model.inc.c"
#include "levels/bbh/areas/1/13/model.inc.c"
#include "levels/bbh/areas/1/14/model.inc.c"
#include "levels/bbh/areas/1/15/model.inc.c"
#include "levels/bbh/areas/1/16/model.inc.c"
#include "levels/bbh/areas/1/17/model.inc.c"
#include "levels/bbh/areas/1/18/model.inc.c"
#include "levels/bbh/areas/1/19/model.inc.c"
#include "levels/bbh/areas/1/20/model.inc.c"
#include "levels/bbh/areas/1/21/model.inc.c"
#include "levels/bbh/areas/1/22/model.inc.c"
#include "levels/bbh/areas/1/23/model.inc.c"
#include "levels/bbh/areas/1/24/model.inc.c"
#include "levels/bbh/areas/1/25/model.inc.c"
#include "levels/bbh/areas/1/26/model.inc.c"
#include "levels/bbh/areas/1/27/model.inc.c"
#include "levels/bbh/areas/1/28/model.inc.c"
#include "levels/bbh/areas/1/29/model.inc.c"
#include "levels/bbh/areas/1/30/model.inc.c"
#include "levels/bbh/areas/1/31/model.inc.c"
#include "levels/bbh/areas/1/32/model.inc.c"
#include "levels/bbh/areas/1/33/model.inc.c"
#include "levels/bbh/areas/1/34/model.inc.c"
#include "levels/bbh/areas/1/35/model.inc.c"
#include "levels/bbh/areas/1/36/model.inc.c"
#include "levels/bbh/areas/1/37/model.inc.c"
#include "levels/bbh/areas/1/38/model.inc.c"
#include "levels/bbh/areas/1/39/model.inc.c"
#include "levels/bbh/staircase_step/model.inc.c"
#include "levels/bbh/tilting_trap_platform/model.inc.c"
#include "levels/bbh/tumbling_platform_far/model.inc.c"
#include "levels/bbh/tumbling_platform_near/model.inc.c"
#include "levels/bbh/moving_bookshelf/model.inc.c"
#include "levels/bbh/mesh_elevator/model.inc.c"
#include "levels/bbh/merry_go_round/model.inc.c"
#include "levels/bbh/coffin/model.inc.c"
#include "levels/bbh/areas/1/collision.inc.c"
#include "levels/bbh/areas/1/room.inc.c"
#include "levels/bbh/areas/1/macro.inc.c"
#include "levels/bbh/staircase_step/collision.inc.c"
#include "levels/bbh/tilting_trap_platform/collision.inc.c"
#include "levels/bbh/tumbling_platform_far/collision.inc.c"
#include "levels/bbh/tumbling_platform_near/collision.inc.c"
#include "levels/bbh/moving_bookshelf/collision.inc.c"
#include "levels/bbh/mesh_elevator/collision.inc.c"
#include "levels/bbh/merry_go_round/collision.inc.c"
#include "levels/bbh/coffin/collision.inc.c"
#include "levels/bbh/areas/1/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(bbh_seg7_dl_07006AF0);
	EXPORT_DL(bbh_seg7_dl_07006DF8);
	EXPORT_DL(bbh_seg7_dl_07006E88);
	EXPORT_DL(bbh_seg7_dl_07006EE0);
	EXPORT_DL(bbh_seg7_dl_070072F8);
	EXPORT_DL(bbh_seg7_dl_07007468);
	EXPORT_DL(bbh_seg7_dl_070074F0);
	EXPORT_DL(bbh_seg7_dl_070075A8);
	EXPORT_DL(bbh_seg7_dl_070078A0);
	EXPORT_DL(bbh_seg7_dl_07007940);
	EXPORT_DL(bbh_seg7_dl_07007AE0);
	EXPORT_DL(bbh_seg7_dl_07007B48);
	EXPORT_DL(bbh_seg7_dl_07007B90);
	EXPORT_DL(bbh_seg7_dl_07007EE8);
	EXPORT_DL(bbh_seg7_dl_07007FD0);
	EXPORT_DL(bbh_seg7_dl_070088E0);
	EXPORT_DL(bbh_seg7_dl_07008B58);
	EXPORT_DL(bbh_seg7_dl_07008DB8);
	EXPORT_DL(bbh_seg7_dl_07008EA8);
	EXPORT_DL(bbh_seg7_dl_0700A800);
	EXPORT_DL(bbh_seg7_dl_0700AA08);
	EXPORT_DL(bbh_seg7_dl_0700ACB0);
	EXPORT_DL(bbh_seg7_dl_0700AE20);
	EXPORT_DL(bbh_seg7_dl_0700AE68);
	EXPORT_DL(bbh_seg7_dl_0700AEB0);
	EXPORT_DL(bbh_seg7_dl_0700AF40);
	EXPORT_DL(bbh_seg7_dl_0700AFF0);
	EXPORT_DL(bbh_seg7_dl_0700B170);
	EXPORT_DL(bbh_seg7_dl_0700B1C8);
	EXPORT_DL(bbh_seg7_dl_0700B398);
	EXPORT_DL(bbh_seg7_dl_0700B418);
	EXPORT_DL(bbh_seg7_dl_0700B8A8);
	EXPORT_DL(bbh_seg7_dl_0700B9E0);
	EXPORT_DL(bbh_seg7_dl_0700BB50);
	EXPORT_DL(bbh_seg7_dl_0700BBF8);
	EXPORT_DL(bbh_seg7_dl_0700CB58);
	EXPORT_DL(bbh_seg7_dl_0700CCB0);
	EXPORT_DL(bbh_seg7_dl_0700CD18);
	EXPORT_DL(bbh_seg7_dl_0700CF40);
	EXPORT_DL(bbh_seg7_dl_0700CFA8);
	EXPORT_DL(bbh_seg7_dl_0700CFE0);
	EXPORT_DL(bbh_seg7_dl_0700D080);
	EXPORT_DL(bbh_seg7_dl_0700D278);
	EXPORT_DL(bbh_seg7_dl_0700D2E0);
	EXPORT_DL(bbh_seg7_dl_0700D428);
	EXPORT_DL(bbh_seg7_dl_0700D490);
	EXPORT_DL(bbh_seg7_dl_0700D6F0);
	EXPORT_DL(bbh_seg7_dl_0700D7E0);
	EXPORT_DL(bbh_seg7_dl_0700EE98);
	EXPORT_DL(bbh_seg7_dl_0700EFD8);
	EXPORT_DL(bbh_seg7_dl_0700F2B8);
	EXPORT_DL(bbh_seg7_dl_0700F3D8);
	EXPORT_DL(bbh_seg7_dl_0700F510);
	EXPORT_DL(bbh_seg7_dl_0700F7A8);
	EXPORT_DL(bbh_seg7_dl_0700F848);
	EXPORT_DL(bbh_seg7_dl_07010A48);
	EXPORT_DL(bbh_seg7_dl_07010B58);
	EXPORT_DL(bbh_seg7_dl_07010C30);
	EXPORT_DL(bbh_seg7_dl_07010D00);
	EXPORT_DL(bbh_seg7_dl_07010D90);
	EXPORT_DL(bbh_seg7_dl_07011058);
	EXPORT_DL(bbh_seg7_dl_070110A0);
	EXPORT_DL(bbh_seg7_dl_070110E8);
	EXPORT_DL(bbh_seg7_dl_07011120);
	EXPORT_DL(bbh_seg7_dl_07011DF0);
	EXPORT_DL(bbh_seg7_dl_07011E48);
	EXPORT_DL(bbh_seg7_dl_07011EA0);
	EXPORT_DL(bbh_seg7_dl_07012078);
	EXPORT_DL(bbh_seg7_dl_070120E0);
	EXPORT_DL(bbh_seg7_dl_070121A8);
	EXPORT_DL(bbh_seg7_dl_07012220);
	EXPORT_DL(bbh_seg7_dl_07012470);
	EXPORT_DL(bbh_seg7_dl_07012510);
	EXPORT_DL(bbh_seg7_dl_07012680);
	EXPORT_DL(bbh_seg7_dl_070126E8);
	EXPORT_DL(bbh_seg7_dl_07013508);
	EXPORT_DL(bbh_seg7_dl_07013550);
	EXPORT_DL(bbh_seg7_dl_070135A8);
	EXPORT_DL(bbh_seg7_dl_070135E0);
	EXPORT_DL(bbh_seg7_dl_070137E0);
	EXPORT_DL(bbh_seg7_dl_07013838);
	EXPORT_DL(bbh_seg7_dl_070139A8);
	EXPORT_DL(bbh_seg7_dl_07013B80);
	EXPORT_DL(bbh_seg7_dl_07013BE8);
	EXPORT_DL(bbh_seg7_dl_07014B28);
	EXPORT_DL(bbh_seg7_dl_07014BC8);
	EXPORT_DL(bbh_seg7_dl_07014C10);
	EXPORT_DL(bbh_seg7_dl_07014C58);
	EXPORT_DL(bbh_seg7_dl_07014D10);
	EXPORT_DL(bbh_seg7_dl_07014FD8);
	EXPORT_DL(bbh_seg7_dl_07015298);
	EXPORT_DL(bbh_seg7_dl_07015398);
	EXPORT_DL(bbh_seg7_dl_07015628);
	EXPORT_DL(bbh_seg7_dl_070156E0);
	EXPORT_DL(bbh_seg7_dl_07015930);
	EXPORT_DL(bbh_seg7_dl_07015A20);
	EXPORT_DL(bbh_seg7_dl_07016D38);
	EXPORT_DL(bbh_seg7_dl_07016DC8);
	EXPORT_DL(bbh_seg7_dl_07016E10);
	EXPORT_DL(bbh_seg7_dl_07016FE8);
	EXPORT_DL(bbh_seg7_dl_07017050);
	EXPORT_DL(bbh_seg7_dl_070170B8);
	EXPORT_DL(bbh_seg7_dl_07017100);
	EXPORT_DL(bbh_seg7_dl_07017148);
	EXPORT_DL(bbh_seg7_dl_07017378);
	EXPORT_DL(bbh_seg7_dl_07017518);
	EXPORT_DL(bbh_seg7_dl_07017570);
	EXPORT_DL(bbh_seg7_dl_070176E0);
	EXPORT_DL(bbh_seg7_dl_07017788);
	EXPORT_DL(bbh_seg7_dl_070195A8);
	EXPORT_DL(bbh_seg7_dl_07019728);
	EXPORT_DL(bbh_seg7_dl_070197B8);
	EXPORT_DL(bbh_seg7_dl_070198C8);
	EXPORT_DL(bbh_seg7_dl_07019B80);
	EXPORT_DL(bbh_seg7_dl_07019BD8);
	EXPORT_DL(bbh_seg7_dl_07019DD0);
	EXPORT_DL(bbh_seg7_dl_07019E60);
	EXPORT_DL(bbh_seg7_dl_07019EA8);
	EXPORT_DL(bbh_seg7_dl_07019EF8);
	EXPORT_DL(bbh_seg7_dl_0701A048);
	EXPORT_DL(bbh_seg7_dl_0701A080);
	EXPORT_DL(bbh_seg7_dl_0701A620);
	EXPORT_DL(bbh_seg7_dl_0701A668);
	EXPORT_DL(bbh_seg7_dl_0701A6E0);
	EXPORT_DL(bbh_seg7_dl_0701A748);
	EXPORT_DL(bbh_seg7_dl_0701A850);
	EXPORT_DL(bbh_seg7_dl_0701B328);
	EXPORT_DL(bbh_seg7_dl_0701B380);
	EXPORT_DL(bbh_seg7_dl_0701B558);
	EXPORT_DL(bbh_seg7_dl_0701B620);
	EXPORT_DL(bbh_seg7_dl_0701B678);
	EXPORT_DL(bbh_seg7_dl_0701B6D0);
	EXPORT_DL(bbh_seg7_dl_0701D9C0);
	EXPORT_DL(bbh_seg7_dl_0701DA50);
	EXPORT_DL(bbh_seg7_dl_0701DB20);
	EXPORT_DL(bbh_seg7_dl_0701DBF0);
	EXPORT_DL(bbh_seg7_dl_0701DFA0);
	EXPORT_DL(bbh_seg7_dl_0701E188);
	EXPORT_DL(bbh_seg7_dl_0701E210);
	EXPORT_DL(bbh_seg7_dl_0701E430);
	EXPORT_DL(bbh_seg7_dl_0701E4B0);
	EXPORT_DL(bbh_seg7_dl_0701E4E0);
	EXPORT_DL(bbh_seg7_dl_0701E810);
	EXPORT_DL(bbh_seg7_dl_0701E8D8);
	EXPORT_DL(bbh_seg7_dl_0701EC18);
	EXPORT_DL(bbh_seg7_dl_0701ED18);
	EXPORT_DL(bbh_seg7_dl_0701EE48);
	EXPORT_DL(bbh_seg7_dl_0701EE90);
	EXPORT_DL(bbh_seg7_dl_0701EEC8);
	EXPORT_DL(bbh_seg7_dl_0701F018);
	EXPORT_DL(bbh_seg7_dl_0701F070);
	EXPORT_DL(bbh_seg7_dl_0701F238);
	EXPORT_DL(bbh_seg7_dl_0701F280);
	EXPORT_DL(bbh_seg7_dl_0701F2E8);
	EXPORT_DL(bbh_seg7_dl_0701F510);
	EXPORT_DL(bbh_seg7_dl_0701F558);
	EXPORT_DL(bbh_seg7_dl_0701F5C0);
	EXPORT_DL(bbh_seg7_dl_0701F5F8);
	EXPORT_DL(bbh_seg7_dl_0701F780);
	EXPORT_DL(bbh_seg7_dl_0701F7E8);
	EXPORT_DL(bbh_seg7_dl_0701FA10);
	EXPORT_DL(bbh_seg7_dl_0701FAB0);
	EXPORT_DL(bbh_seg7_dl_0701FC78);
	EXPORT_DL(bbh_seg7_dl_0701FCC0);
	EXPORT_DL(bbh_seg7_dl_0701FD28);
	EXPORT_DL(bbh_seg7_dl_0701FF58);
	EXPORT_DL(bbh_seg7_dl_0701FFE8);
	EXPORT_DL(bbh_seg7_dl_07020230);
	EXPORT_DL(bbh_seg7_dl_07020288);
	EXPORT_DL(bbh_seg7_dl_070202F0);
	EXPORT_DL(bbh_seg7_dl_070205C0);
	EXPORT_DL(bbh_seg7_dl_07020660);
	EXPORT_DL(bbh_seg7_dl_070206A8);
	EXPORT_DL(bbh_seg7_dl_070206F0);
}