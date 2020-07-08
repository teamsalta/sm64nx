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
#include "levels/castle_inside/texture.inc.c"
#include "levels/castle_inside/painting.inc.c"
#include "levels/castle_inside/areas/1/1/model.inc.c"
#include "levels/castle_inside/areas/1/2/model.inc.c"
#include "levels/castle_inside/areas/1/3/model.inc.c"
#include "levels/castle_inside/areas/1/4/model.inc.c"
#include "levels/castle_inside/areas/1/5/model.inc.c"
#include "levels/castle_inside/areas/1/6/model.inc.c"
#include "levels/castle_inside/areas/1/7/model.inc.c"
#include "levels/castle_inside/areas/1/8/model.inc.c"
#include "levels/castle_inside/areas/1/9/model.inc.c"
#include "levels/castle_inside/areas/1/10/model.inc.c"
#include "levels/castle_inside/areas/1/11/model.inc.c"
#include "levels/castle_inside/areas/1/12/model.inc.c"
#include "levels/castle_inside/areas/1/13/model.inc.c"
#include "levels/castle_inside/areas/1/14/model.inc.c"
#include "levels/castle_inside/areas/1/15/model.inc.c"
#include "levels/castle_inside/areas/1/16/model.inc.c"
#include "levels/castle_inside/areas/1/17/model.inc.c"
#include "levels/castle_inside/areas/1/18/model.inc.c"
#include "levels/castle_inside/areas/1/19/model.inc.c"
#include "levels/castle_inside/areas/1/20/model.inc.c"
#include "levels/castle_inside/areas/1/21/model.inc.c"
#include "levels/castle_inside/areas/1/22/model.inc.c"
#include "levels/castle_inside/areas/1/23/model.inc.c"
#include "levels/castle_inside/areas/1/24/model.inc.c"
#include "levels/castle_inside/areas/1/25/model.inc.c"
#include "levels/castle_inside/areas/1/26/model.inc.c"
#include "levels/castle_inside/trap_door/model.inc.c"
#include "levels/castle_inside/star_door/model.inc.c"
#include "levels/castle_inside/areas/2/1/model.inc.c"
#include "levels/castle_inside/areas/2/2/model.inc.c"
#include "levels/castle_inside/areas/2/3/model.inc.c"
#include "levels/castle_inside/areas/2/4/model.inc.c"
#include "levels/castle_inside/areas/2/5/model.inc.c"
#include "levels/castle_inside/areas/2/6/model.inc.c"
#include "levels/castle_inside/areas/2/7/model.inc.c"
#include "levels/castle_inside/areas/2/8/model.inc.c"
#include "levels/castle_inside/areas/2/9/model.inc.c"
#include "levels/castle_inside/areas/2/pendulum/model.inc.c"
#include "levels/castle_inside/areas/2/10/model.inc.c"
#include "levels/castle_inside/areas/2/11/model.inc.c"
#include "levels/castle_inside/areas/2/12/model.inc.c"
#include "levels/castle_inside/areas/2/13/model.inc.c"
#include "levels/castle_inside/areas/2/14/model.inc.c"
#include "levels/castle_inside/areas/2/15/model.inc.c"
#include "levels/castle_inside/clock_minute_hand/model.inc.c"
#include "levels/castle_inside/clock_hour_hand/model.inc.c"
#include "levels/castle_inside/areas/3/1/model.inc.c"
#include "levels/castle_inside/areas/3/2/model.inc.c"
#include "levels/castle_inside/areas/3/3/model.inc.c"
#include "levels/castle_inside/areas/3/4/model.inc.c"
#include "levels/castle_inside/areas/3/5/model.inc.c"
#include "levels/castle_inside/areas/3/6/model.inc.c"
#include "levels/castle_inside/areas/3/7/model.inc.c"
#include "levels/castle_inside/areas/3/8/model.inc.c"
#include "levels/castle_inside/areas/3/9/model.inc.c"
#include "levels/castle_inside/areas/3/10/model.inc.c"
#include "levels/castle_inside/areas/3/11/model.inc.c"
#include "levels/castle_inside/water_level_pillar/model.inc.c"
#include "levels/castle_inside/areas/1/collision.inc.c"
#include "levels/castle_inside/areas/2/collision.inc.c"
#include "levels/castle_inside/areas/3/collision.inc.c"
#include "levels/castle_inside/areas/1/macro.inc.c"
#include "levels/castle_inside/areas/2/macro.inc.c"
#include "levels/castle_inside/areas/3/macro.inc.c"
#include "levels/castle_inside/areas/1/room.inc.c"
#include "levels/castle_inside/areas/2/room.inc.c"
#include "levels/castle_inside/areas/3/room.inc.c"
#include "levels/castle_inside/trap_door/collision.inc.c"
#include "levels/castle_inside/star_door/collision.inc.c"
#include "levels/castle_inside/clock_minute_hand/collision.inc.c"
#include "levels/castle_inside/clock_hour_hand/collision.inc.c"
#include "levels/castle_inside/water_level_pillar/collision.inc.c"
#include "levels/castle_inside/areas/3/trajectory.inc.c"
#include "levels/castle_inside/areas/3/movtext.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(inside_castle_seg7_dl_07021A18);
	EXPORT_DL(inside_castle_seg7_dl_07021A30);
	EXPORT_DL(inside_castle_seg7_dl_07021A48);
	EXPORT_DL(inside_castle_seg7_dl_07021AA0);
	EXPORT_DL(inside_castle_seg7_painting_dl_07021AC0);
	EXPORT_DL(inside_castle_seg7_dl_070225D8);
	EXPORT_DL(inside_castle_seg7_dl_07022610);
	EXPORT_DL(inside_castle_seg7_painting_dl_07022640);
	EXPORT_DL(inside_castle_seg7_painting_dl_07023050);
	EXPORT_DL(inside_castle_seg7_painting_dl_070230B0);
	EXPORT_DL(inside_castle_seg7_painting_dl_07023110);
	EXPORT_DL(inside_castle_seg7_painting_dl_07023170);
	EXPORT_DL(inside_castle_seg7_painting_dl_070231D0);
	EXPORT_DL(inside_castle_seg7_painting_dl_07023230);
	EXPORT_DL(inside_castle_seg7_painting_dl_07023290);
	EXPORT_DL(inside_castle_seg7_painting_dl_070232F0);
	EXPORT_DL(inside_castle_seg7_painting_dl_07023350);
	EXPORT_DL(inside_castle_seg7_painting_dl_070233B0);
	EXPORT_DL(inside_castle_seg7_painting_dl_07023410);
	EXPORT_DL(inside_castle_seg7_dl_070234C0);
	EXPORT_DL(inside_castle_seg7_dl_07023520);
	EXPORT_DL(inside_castle_seg7_painting_dl_07023580);
	EXPORT_DL(inside_castle_seg7_painting_dl_070235B8);
	EXPORT_DL(inside_castle_seg7_dl_07023DB0);
	EXPORT_DL(inside_castle_seg7_dl_07027DE8);
	EXPORT_DL(inside_castle_seg7_dl_07028418);
	EXPORT_DL(inside_castle_seg7_dl_070286C0);
	EXPORT_DL(inside_castle_seg7_dl_070287C0);
	EXPORT_DL(inside_castle_seg7_dl_07028A38);
	EXPORT_DL(inside_castle_seg7_dl_07028FD0);
	EXPORT_DL(inside_castle_seg7_dl_07029438);
	EXPORT_DL(inside_castle_seg7_dl_07029578);
	EXPORT_DL(inside_castle_seg7_dl_0702A188);
	EXPORT_DL(inside_castle_seg7_dl_0702A300);
	EXPORT_DL(inside_castle_seg7_dl_0702A548);
	EXPORT_DL(inside_castle_seg7_dl_0702A650);
	EXPORT_DL(dl_castle_lobby_wing_cap_light_model);
	EXPORT_DL(dl_castle_lobby_wing_cap_light);
	EXPORT_DL(inside_castle_sun);
	EXPORT_DL(inside_castle_seg7_dl_0702AA10);
	EXPORT_DL(inside_castle_seg7_dl_0702AAD8);
	EXPORT_DL(inside_castle_seg7_dl_0702AB20);
	EXPORT_DL(inside_castle_seg7_dl_0702D770);
	EXPORT_DL(inside_castle_seg7_dl_0702DE40);
	EXPORT_DL(inside_castle_seg7_dl_0702DE88);
	EXPORT_DL(inside_castle_seg7_dl_0702DF38);
	EXPORT_DL(inside_castle_seg7_dl_0702E408);
	EXPORT_DL(inside_castle_seg7_dl_0702F738);
	EXPORT_DL(inside_castle_seg7_dl_0702F8D0);
	EXPORT_DL(inside_castle_seg7_dl_0702F918);
	EXPORT_DL(inside_castle_seg7_dl_0702FA80);
	EXPORT_DL(inside_castle_seg7_dl_0702FC90);
	EXPORT_DL(inside_castle_seg7_dl_0702FD30);
	EXPORT_DL(inside_castle_seg7_dl_07031070);
	EXPORT_DL(inside_castle_seg7_dl_070310D8);
	EXPORT_DL(inside_castle_seg7_dl_07031168);
	EXPORT_DL(inside_castle_seg7_dl_07031588);
	EXPORT_DL(inside_castle_seg7_dl_070316C8);
	EXPORT_DL(inside_castle_seg7_dl_07031720);
	EXPORT_DL(inside_castle_seg7_dl_070317E8);
	EXPORT_DL(inside_castle_seg7_dl_07031830);
	EXPORT_DL(inside_castle_seg7_dl_07032A00);
	EXPORT_DL(inside_castle_seg7_dl_07032B18);
	EXPORT_DL(inside_castle_seg7_dl_07032C18);
	EXPORT_DL(inside_castle_seg7_dl_07032DC8);
	EXPORT_DL(inside_castle_seg7_dl_07032EC0);
	EXPORT_DL(inside_castle_seg7_dl_07032FC0);
	EXPORT_DL(inside_castle_seg7_dl_07033100);
	EXPORT_DL(inside_castle_seg7_dl_07033158);
	EXPORT_DL(inside_castle_seg7_dl_07034688);
	EXPORT_DL(inside_castle_seg7_dl_070347D8);
	EXPORT_DL(inside_castle_seg7_dl_07034888);
	EXPORT_DL(inside_castle_seg7_dl_07034B28);
	EXPORT_DL(inside_castle_seg7_dl_07034B90);
	EXPORT_DL(inside_castle_seg7_dl_07034C90);
	EXPORT_DL(inside_castle_seg7_dl_07034D88);
	EXPORT_DL(inside_castle_seg7_dl_070350B0);
	EXPORT_DL(inside_castle_seg7_dl_07035178);
	EXPORT_DL(inside_castle_seg7_dl_07035240);
	EXPORT_DL(inside_castle_seg7_dl_07035288);
	EXPORT_DL(inside_castle_seg7_dl_07036698);
	EXPORT_DL(inside_castle_seg7_dl_07036780);
	EXPORT_DL(inside_castle_seg7_dl_070367C8);
	EXPORT_DL(inside_castle_seg7_dl_07036850);
	EXPORT_DL(inside_castle_seg7_dl_070369B8);
	EXPORT_DL(inside_castle_seg7_dl_07036CB8);
	EXPORT_DL(inside_castle_seg7_dl_07036D88);
	EXPORT_DL(inside_castle_seg7_dl_07037710);
	EXPORT_DL(inside_castle_seg7_dl_07037988);
	EXPORT_DL(inside_castle_seg7_dl_07037B10);
	EXPORT_DL(inside_castle_seg7_dl_07037BF8);
	EXPORT_DL(inside_castle_seg7_dl_07037D70);
	EXPORT_DL(inside_castle_seg7_dl_07037DE8);
	EXPORT_DL(dl_castle_aquarium_light_model);
	EXPORT_DL(dl_castle_aquarium_light);
	EXPORT_DL(inside_castle_seg7_dl_07038308);
	EXPORT_DL(inside_castle_seg7_dl_07038350);
	EXPORT_DL(inside_castle_seg7_dl_07039EC8);
	EXPORT_DL(inside_castle_seg7_dl_0703A058);
	EXPORT_DL(inside_castle_seg7_dl_0703A178);
	EXPORT_DL(inside_castle_seg7_dl_0703A540);
	EXPORT_DL(inside_castle_seg7_dl_0703A6C8);
	EXPORT_DL(inside_castle_seg7_dl_0703A7C0);
	EXPORT_DL(inside_castle_seg7_dl_0703A808);
	EXPORT_DL(inside_castle_seg7_dl_0703B5B8);
	EXPORT_DL(inside_castle_seg7_dl_0703B6C8);
	EXPORT_DL(inside_castle_seg7_dl_0703B740);
	EXPORT_DL(inside_castle_seg7_dl_0703B808);
	EXPORT_DL(inside_castle_seg7_dl_0703B988);
	EXPORT_DL(inside_castle_seg7_dl_0703BA08);
	EXPORT_DL(inside_castle_seg7_dl_0703BC28);
	EXPORT_DL(inside_castle_seg7_dl_0703BCB8);
	EXPORT_DL(inside_castle_seg7_dl_0703BEC0);
	EXPORT_DL(inside_castle_seg7_dl_0703BF38);
	EXPORT_DL(inside_castle_seg7_dl_0703BF70);
	EXPORT_DL(inside_castle_seg7_dl_0703BFA8);
	EXPORT_DL(inside_castle_seg7_dl_0703DE40);
	EXPORT_DL(inside_castle_seg7_dl_0703DF70);
	EXPORT_DL(inside_castle_seg7_dl_0703E000);
	EXPORT_DL(inside_castle_seg7_dl_0703E2D8);
	EXPORT_DL(inside_castle_seg7_dl_0703E618);
	EXPORT_DL(inside_castle_seg7_dl_0703E6F0);
	EXPORT_DL(inside_castle_seg7_dl_07041F30);
	EXPORT_DL(inside_castle_seg7_dl_07042050);
	EXPORT_DL(inside_castle_seg7_dl_070421E8);
	EXPORT_DL(inside_castle_seg7_dl_07042828);
	EXPORT_DL(inside_castle_seg7_dl_07042BD0);
	EXPORT_DL(inside_castle_seg7_dl_07042CC8);
	EXPORT_DL(inside_castle_seg7_dl_07042DC8);
	EXPORT_DL(inside_castle_seg7_dl_07043028);
	EXPORT_DL(inside_castle_seg7_dl_070437C8);
	EXPORT_DL(inside_castle_seg7_dl_07043820);
	EXPORT_DL(inside_castle_seg7_dl_07043930);
	EXPORT_DL(inside_castle_seg7_dl_07043988);
	EXPORT_DL(inside_castle_seg7_dl_070439C0);
	EXPORT_DL(inside_castle_seg7_dl_070439F8);
	EXPORT_DL(inside_castle_seg7_dl_07043A30);
	EXPORT_DL(inside_castle_seg7_dl_07043A68);
	EXPORT_DL(inside_castle_seg7_dl_07043AA0);
	EXPORT_DL(inside_castle_seg7_dl_07043AD8);
	EXPORT_DL(inside_castle_seg7_dl_07043B10);
	EXPORT_DL(inside_castle_seg7_dl_07043B48);
	EXPORT_DL(inside_castle_seg7_dl_07043C90);
	EXPORT_DL(inside_castle_seg7_dl_07043CD8);
	EXPORT_DL(inside_castle_seg7_dl_07048A38);
	EXPORT_DL(inside_castle_seg7_dl_07048AA0);
	EXPORT_DL(inside_castle_seg7_dl_07048E68);
	EXPORT_DL(inside_castle_seg7_dl_07048F68);
	EXPORT_DL(inside_castle_seg7_dl_07049248);
	EXPORT_DL(inside_castle_seg7_dl_070494D8);
	EXPORT_DL(inside_castle_seg7_dl_07049E98);
	EXPORT_DL(inside_castle_seg7_dl_0704A0E8);
	EXPORT_DL(inside_castle_seg7_dl_0704A290);
	EXPORT_DL(inside_castle_seg7_dl_0704A2E0);
	EXPORT_DL(inside_castle_seg7_dl_0704A7E8);
	EXPORT_DL(inside_castle_seg7_dl_0704A830);
	EXPORT_DL(inside_castle_seg7_dl_0704A878);
	EXPORT_DL(inside_castle_seg7_dl_0704A8C0);
	EXPORT_DL(inside_castle_seg7_dl_0704A908);
	EXPORT_DL(inside_castle_seg7_dl_0704A950);
	EXPORT_DL(inside_castle_seg7_dl_0704A998);
	EXPORT_DL(inside_castle_seg7_dl_0704A9E0);
	EXPORT_DL(inside_castle_seg7_dl_0704AA28);
	EXPORT_DL(inside_castle_seg7_dl_0704AA60);
	EXPORT_DL(inside_castle_seg7_dl_0704AA98);
	EXPORT_DL(inside_castle_seg7_dl_0704C078);
	EXPORT_DL(inside_castle_seg7_dl_0704C378);
	EXPORT_DL(inside_castle_seg7_dl_0704C528);
	EXPORT_DL(inside_castle_seg7_dl_0704C5F0);
	EXPORT_DL(inside_castle_seg7_dl_0704C638);
	EXPORT_DL(inside_castle_seg7_dl_0704C6B8);
	EXPORT_DL(inside_castle_seg7_dl_0704C768);
	EXPORT_DL(inside_castle_seg7_dl_0704C7A0);
	EXPORT_DL(inside_castle_seg7_dl_0704C7D8);
	EXPORT_DL(inside_castle_seg7_dl_0704F928);
	EXPORT_DL(inside_castle_seg7_dl_0704FB98);
	EXPORT_DL(inside_castle_seg7_dl_0704FED8);
	EXPORT_DL(inside_castle_seg7_dl_0704FF60);
	EXPORT_DL(inside_castle_seg7_dl_0704FFA8);
	EXPORT_DL(inside_castle_seg7_dl_07050080);
	EXPORT_DL(inside_castle_seg7_dl_070501E8);
	EXPORT_DL(inside_castle_seg7_dl_07050230);
	EXPORT_DL(inside_castle_seg7_dl_07050278);
	EXPORT_DL(inside_castle_seg7_dl_070502C0);
	EXPORT_DL(inside_castle_seg7_dl_07050308);
	EXPORT_DL(inside_castle_seg7_dl_070505D0);
	EXPORT_DL(inside_castle_seg7_dl_07050938);
	EXPORT_DL(inside_castle_seg7_dl_07051108);
	EXPORT_DL(inside_castle_seg7_dl_07051170);
	EXPORT_DL(inside_castle_seg7_dl_070512F8);
	EXPORT_DL(inside_castle_seg7_dl_070515B0);
	EXPORT_DL(inside_castle_seg7_dl_07051678);
	EXPORT_DL(inside_castle_seg7_dl_070518D8);
	EXPORT_DL(inside_castle_seg7_dl_070519C8);
	EXPORT_DL(inside_castle_seg7_dl_07051B10);
	EXPORT_DL(inside_castle_seg7_dl_07051B60);
	EXPORT_DL(inside_castle_seg7_dl_07054AF0);
	EXPORT_DL(inside_castle_seg7_dl_07054B78);
	EXPORT_DL(inside_castle_seg7_dl_070555E8);
	EXPORT_DL(inside_castle_seg7_dl_070557A8);
	EXPORT_DL(inside_castle_seg7_dl_070558A0);
	EXPORT_DL(inside_castle_seg7_dl_070558D0);
	EXPORT_DL(inside_castle_seg7_dl_07056BF0);
	EXPORT_DL(inside_castle_seg7_dl_07056CE0);
	EXPORT_DL(inside_castle_seg7_dl_07056DD0);
	EXPORT_DL(inside_castle_seg7_dl_07056EC0);
	EXPORT_DL(inside_castle_seg7_dl_07056FB0);
	EXPORT_DL(inside_castle_seg7_dl_07057078);
	EXPORT_DL(inside_castle_seg7_dl_07057130);
	EXPORT_DL(inside_castle_seg7_dl_070571E8);
	EXPORT_DL(inside_castle_seg7_dl_070572A0);
	EXPORT_DL(inside_castle_seg7_dl_07057B70);
	EXPORT_DL(inside_castle_seg7_dl_07057BB8);
	EXPORT_DL(inside_castle_seg7_dl_07057C20);
	EXPORT_DL(inside_castle_seg7_dl_07057D30);
	EXPORT_DL(inside_castle_seg7_dl_07057D88);
	EXPORT_DL(inside_castle_seg7_dl_07057DD0);
	EXPORT_DL(inside_castle_seg7_dl_07057E18);
	EXPORT_DL(inside_castle_seg7_dl_07057E60);
	EXPORT_DL(inside_castle_seg7_dl_07057F00);
	EXPORT_DL(inside_castle_seg7_dl_07058718);
	EXPORT_DL(inside_castle_seg7_dl_07058950);
	EXPORT_DL(inside_castle_seg7_dl_07058FC0);
	EXPORT_DL(inside_castle_seg7_dl_07059190);
	EXPORT_DL(inside_castle_seg7_dl_0705CEA0);
	EXPORT_DL(inside_castle_seg7_dl_0705D098);
	EXPORT_DL(inside_castle_seg7_dl_0705D480);
	EXPORT_DL(inside_castle_seg7_dl_0705D550);
	EXPORT_DL(inside_castle_seg7_dl_0705DAD0);
	EXPORT_DL(inside_castle_seg7_dl_0705DD60);
	EXPORT_DL(inside_castle_seg7_dl_0705E088);
	EXPORT_DL(inside_castle_seg7_dl_0705E238);
	EXPORT_DL(inside_castle_seg7_dl_0705E2A0);
	EXPORT_DL(inside_castle_seg7_dl_0705E3E8);
	EXPORT_DL(inside_castle_seg7_dl_0705E450);
	EXPORT_DL(inside_castle_seg7_dl_07060B58);
	EXPORT_DL(inside_castle_seg7_dl_07060C68);
	EXPORT_DL(inside_castle_seg7_dl_07060E70);
	EXPORT_DL(inside_castle_seg7_dl_07060EF8);
	EXPORT_DL(inside_castle_seg7_dl_07061188);
	EXPORT_DL(inside_castle_seg7_dl_070616E8);
	EXPORT_DL(inside_castle_seg7_dl_07061B10);
	EXPORT_DL(inside_castle_seg7_dl_07061C20);
	EXPORT_DL(inside_castle_seg7_dl_07064078);
	EXPORT_DL(inside_castle_seg7_dl_07064180);
	EXPORT_DL(inside_castle_seg7_dl_07064618);
	EXPORT_DL(inside_castle_seg7_dl_07064A50);
	EXPORT_DL(inside_castle_seg7_dl_07064A88);
	EXPORT_DL(inside_castle_seg7_dl_07064B28);
	EXPORT_DL(inside_castle_seg7_dl_07064B78);
	EXPORT_DL(inside_castle_seg7_dl_07064D00);
	EXPORT_DL(inside_castle_seg7_dl_07064D58);
	EXPORT_DL(inside_castle_seg7_dl_07066560);
	EXPORT_DL(inside_castle_seg7_dl_07066768);
	EXPORT_DL(inside_castle_seg7_dl_07066838);
	EXPORT_DL(inside_castle_seg7_dl_07066910);
	EXPORT_DL(inside_castle_seg7_dl_07066BE8);
	EXPORT_DL(inside_castle_seg7_dl_07066CE0);
	EXPORT_DL(inside_castle_seg7_dl_07066E48);
	EXPORT_DL(inside_castle_seg7_dl_07066E90);
	EXPORT_DL(inside_castle_seg7_dl_07066F58);
	EXPORT_DL(inside_castle_seg7_dl_07066FA0);
	EXPORT_DL(inside_castle_seg7_dl_07068210);
	EXPORT_DL(inside_castle_seg7_dl_07068288);
	EXPORT_DL(inside_castle_seg7_dl_070682F0);
	EXPORT_DL(inside_castle_seg7_dl_070683F0);
	EXPORT_DL(inside_castle_seg7_dl_070684E8);
	EXPORT_DL(inside_castle_seg7_dl_070687A8);
	EXPORT_DL(inside_castle_seg7_dl_07068820);
	EXPORT_DL(inside_castle_seg7_dl_07068850);
	EXPORT_DL(inside_castle_seg7_dl_07068A60);
	EXPORT_DL(inside_castle_seg7_dl_07068AA8);
	EXPORT_DL(inside_castle_seg7_dl_07068B10);
}
