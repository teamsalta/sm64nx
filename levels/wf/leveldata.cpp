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
#include "levels/wf/texture.inc.c"
#include "levels/wf/areas/1/1/model.inc.c"
#include "levels/wf/areas/1/2/model.inc.c"
#include "levels/wf/areas/1/3/model.inc.c"
#include "levels/wf/areas/1/4/model.inc.c"
#include "levels/wf/areas/1/5/1.inc.c"
#include "levels/wf/areas/1/5/2.inc.c"
#include "levels/wf/areas/1/6/1.inc.c"
#include "levels/wf/areas/1/6/2.inc.c"
#include "levels/wf/areas/1/7/model.inc.c"
#include "levels/wf/areas/1/8/model.inc.c"
#include "levels/wf/areas/1/9/model.inc.c"
#include "levels/wf/areas/1/10/1.inc.c"
#include "levels/wf/areas/1/10/2.inc.c"
#include "levels/wf/areas/1/11/model.inc.c"
#include "levels/wf/areas/1/12/1.inc.c"
#include "levels/wf/areas/1/12/2.inc.c"
#include "levels/wf/areas/1/13/model.inc.c"
#include "levels/wf/areas/1/14/model.inc.c"
#include "levels/wf/areas/1/15/1.inc.c"
#include "levels/wf/areas/1/15/2.inc.c"
#include "levels/wf/areas/1/15/3.inc.c"
#include "levels/wf/areas/1/16/model.inc.c"
#include "levels/wf/areas/1/17/model.inc.c"
#include "levels/wf/areas/1/18/model.inc.c"
#include "levels/wf/areas/1/19/model.inc.c"
#include "levels/wf/small_bomp/1.inc.c"
#include "levels/wf/small_bomp/2.inc.c"
#include "levels/wf/large_bomp/model.inc.c"
#include "levels/wf/tumbling_bridge_near/model.inc.c"
#include "levels/wf/tumbling_bridge_far/model.inc.c"
#include "levels/wf/rotating_wooden_platform/1.inc.c"
#include "levels/wf/rotating_wooden_platform/2.inc.c"
#include "levels/wf/sliding_platform/model.inc.c"
#include "levels/wf/beta_extending_platform/model.inc.c"
#include "levels/wf/extending_platform/model.inc.c"
#include "levels/wf/breakable_wall_right/model.inc.c"
#include "levels/wf/breakable_wall_left/model.inc.c"
#include "levels/wf/kickable_board/1.inc.c"
#include "levels/wf/kickable_board/2.inc.c"
#include "levels/wf/tower_door/model.inc.c"
#include "levels/wf/small_bomp/collision.inc.c"
#include "levels/wf/large_bomp/collision.inc.c"
#include "levels/wf/rotating_wooden_platform/collision.inc.c"
#include "levels/wf/sliding_platform/collision.inc.c"
#include "levels/wf/tumbling_bridge_near/collision.inc.c"
#include "levels/wf/beta_extending_platform/collision.inc.c"
#include "levels/wf/extending_platform/collision.inc.c"
#include "levels/wf/breakable_wall_right/collision.inc.c"
#include "levels/wf/breakable_wall_left/collision.inc.c"
#include "levels/wf/kickable_board/collision.inc.c"
#include "levels/wf/tower_door/collision.inc.c"
#include "levels/wf/rotating_platform/collision.inc.c" // This is odd... TODO: investigate this.
#include "levels/wf/areas/1/10/collision.inc.c"
#include "levels/wf/areas/1/11/collision.inc.c"
#include "levels/wf/areas/1/collision.inc.c"
#include "levels/wf/areas/1/macro.inc.c"
#include "levels/wf/areas/1/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(wf_seg7_dl_070046B8);
	EXPORT_DL(wf_seg7_dl_07004758);
	EXPORT_DL(wf_seg7_dl_070047C8);
	EXPORT_DL(wf_seg7_dl_070048D8);
	EXPORT_DL(wf_seg7_dl_07004B30);
	EXPORT_DL(wf_seg7_dl_07004B68);
	EXPORT_DL(wf_seg7_dl_07004C50);
	EXPORT_DL(wf_seg7_dl_07004D08);
	EXPORT_DL(wf_seg7_dl_07004DD8);
	EXPORT_DL(wf_seg7_dl_07004F88);
	EXPORT_DL(wf_seg7_dl_07004FF0);
	EXPORT_DL(wf_seg7_dl_07005038);
	EXPORT_DL(wf_seg7_dl_07005070);
	EXPORT_DL(wf_seg7_dl_070050C8);
	EXPORT_DL(wf_seg7_dl_07005260);
	EXPORT_DL(wf_seg7_dl_070052B8);
	EXPORT_DL(wf_seg7_dl_070054D8);
	EXPORT_DL(wf_seg7_dl_07005538);
	EXPORT_DL(wf_seg7_dl_07005658);
	EXPORT_DL(wf_seg7_dl_07005690);
	EXPORT_DL(wf_seg7_dl_07005C38);
	EXPORT_DL(wf_seg7_dl_07005DD8);
	EXPORT_DL(wf_seg7_dl_07005EA0);
	EXPORT_DL(wf_seg7_dl_07005EE8);
	EXPORT_DL(wf_seg7_dl_070063A0);
	EXPORT_DL(wf_seg7_dl_070064E8);
	EXPORT_DL(wf_seg7_dl_070065B0);
	EXPORT_DL(wf_seg7_dl_070065F8);
	EXPORT_DL(wf_seg7_dl_07006780);
	EXPORT_DL(wf_seg7_dl_070067C8);
	EXPORT_DL(wf_seg7_dl_07006820);
	EXPORT_DL(wf_seg7_dl_07006FB8);
	EXPORT_DL(wf_seg7_dl_07007168);
	EXPORT_DL(wf_seg7_dl_07007220);
	EXPORT_DL(wf_seg7_dl_07007450);
	EXPORT_DL(wf_seg7_dl_070074E0);
	EXPORT_DL(wf_seg7_dl_07007518);
	EXPORT_DL(wf_seg7_dl_07008368);
	EXPORT_DL(wf_seg7_dl_070083E0);
	EXPORT_DL(wf_seg7_dl_07008468);
	EXPORT_DL(wf_seg7_dl_07008690);
	EXPORT_DL(wf_seg7_dl_070087B0);
	EXPORT_DL(wf_seg7_dl_07008DD0);
	EXPORT_DL(wf_seg7_dl_07008E48);
	EXPORT_DL(wf_seg7_dl_07008EA0);
	EXPORT_DL(wf_seg7_dl_07008F58);
	EXPORT_DL(wf_seg7_dl_07008FE8);
	EXPORT_DL(wf_seg7_dl_070091C8);
	EXPORT_DL(wf_seg7_dl_07009210);
	EXPORT_DL(wf_seg7_dl_07009278);
	EXPORT_DL(wf_seg7_dl_07009698);
	EXPORT_DL(wf_seg7_dl_07009780);
	EXPORT_DL(wf_seg7_dl_07009848);
	EXPORT_DL(wf_seg7_dl_07009890);
	EXPORT_DL(wf_seg7_dl_07009C58);
	EXPORT_DL(wf_seg7_dl_07009CA0);
	EXPORT_DL(wf_seg7_dl_07009D20);
	EXPORT_DL(wf_seg7_dl_07009DB0);
	EXPORT_DL(wf_seg7_dl_0700A0A8);
	EXPORT_DL(wf_seg7_dl_0700A170);
	EXPORT_DL(wf_seg7_dl_0700A8F8);
	EXPORT_DL(wf_seg7_dl_0700A988);
	EXPORT_DL(wf_seg7_dl_0700A9D0);
	EXPORT_DL(wf_seg7_dl_0700AB18);
	EXPORT_DL(wf_seg7_dl_0700ABA0);
	EXPORT_DL(wf_seg7_dl_0700ADE8);
	EXPORT_DL(wf_seg7_dl_0700AE40);
	EXPORT_DL(wf_seg7_dl_0700AEC8);
	EXPORT_DL(wf_seg7_dl_0700AF80);
	EXPORT_DL(wf_seg7_dl_0700AFB8);
	EXPORT_DL(wf_seg7_dl_0700B750);
	EXPORT_DL(wf_seg7_dl_0700B790);
	EXPORT_DL(wf_seg7_dl_0700B808);
	EXPORT_DL(wf_seg7_dl_0700B858);
	EXPORT_DL(wf_seg7_dl_0700B9A0);
	EXPORT_DL(wf_seg7_dl_0700BA28);
	EXPORT_DL(wf_seg7_dl_0700BE30);
	EXPORT_DL(wf_seg7_dl_0700BED8);
	EXPORT_DL(wf_seg7_dl_0700BF50);
	EXPORT_DL(wf_seg7_dl_0700CC38);
	EXPORT_DL(wf_seg7_dl_0700CD00);
	EXPORT_DL(wf_seg7_dl_0700CDB0);
	EXPORT_DL(wf_seg7_dl_0700D028);
	EXPORT_DL(wf_seg7_dl_0700D260);
	EXPORT_DL(wf_seg7_dl_0700D300);
	EXPORT_DL(wf_seg7_dl_0700D608);
	EXPORT_DL(wf_seg7_dl_0700D6D0);
	EXPORT_DL(wf_seg7_dl_0700D708);
	EXPORT_DL(wf_seg7_dl_0700D938);
	EXPORT_DL(wf_seg7_dl_0700D9B8);
	EXPORT_DL(wf_seg7_dl_0700D9F0);
	EXPORT_DL(wf_seg7_dl_0700DD20);
	EXPORT_DL(wf_seg7_dl_0700DDE8);
	EXPORT_DL(wf_seg7_dl_0700DE20);
	EXPORT_DL(wf_seg7_dl_0700E050);
	EXPORT_DL(wf_seg7_dl_0700E0F0);
	EXPORT_DL(wf_seg7_dl_0700E328);
	EXPORT_DL(wf_seg7_dl_0700E3D0);
	EXPORT_DL(wf_seg7_dl_0700E698);
	EXPORT_DL(wf_seg7_dl_0700E760);
	EXPORT_DL(wf_seg7_dl_0700E918);
	EXPORT_DL(wf_seg7_dl_0700E9B8);
	EXPORT_DL(wf_seg7_dl_0700EAC0);
	EXPORT_DL(wf_seg7_dl_0700EB08);
	EXPORT_DL(wf_seg7_dl_0700EB40);
	EXPORT_DL(wf_seg7_dl_0700ED30);
	EXPORT_DL(wf_seg7_dl_0700EDC0);
	EXPORT_DL(wf_seg7_dl_0700EF88);
	EXPORT_DL(wf_seg7_dl_0700F018);
	EXPORT_DL(wf_seg7_dl_0700F130);
	EXPORT_DL(wf_seg7_dl_0700F170);
	EXPORT_DL(wf_seg7_dl_0700F1A8);
	EXPORT_DL(wf_seg7_dl_0700F2C8);
	EXPORT_DL(wf_seg7_dl_0700F308);
	EXPORT_DL(wf_seg7_dl_0700F340);
	EXPORT_DL(wf_seg7_dl_0700F570);
	EXPORT_DL(wf_seg7_dl_0700F610);
	EXPORT_DL(wf_seg7_dl_0700F6C0);
	EXPORT_DL(wf_seg7_dl_0700F7B0);
	EXPORT_DL(wf_seg7_dl_0700F7F8);
}
