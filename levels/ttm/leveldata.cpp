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
#include "levels/ttm/texture.inc.c"
#include "levels/ttm/areas/1/1/model.inc.c"
#include "levels/ttm/areas/1/2/model.inc.c"
#include "levels/ttm/areas/1/3/1.inc.c"
#include "levels/ttm/areas/1/3/2.inc.c"
#include "levels/ttm/areas/1/4/1.inc.c"
#include "levels/ttm/areas/1/4/2.inc.c"
#include "levels/ttm/areas/1/5/1.inc.c"
#include "levels/ttm/areas/1/5/2.inc.c"
#include "levels/ttm/areas/1/6/1.inc.c"
#include "levels/ttm/areas/1/6/2.inc.c"
#include "levels/ttm/areas/1/7/model.inc.c"
#include "levels/ttm/areas/1/8/model.inc.c"
#include "levels/ttm/areas/1/9/model.inc.c"
#include "levels/ttm/areas/1/10/model.inc.c"
#include "levels/ttm/areas/1/11/model.inc.c"
#include "levels/ttm/areas/1/12/model.inc.c"
#include "levels/ttm/areas/1/13/model.inc.c"
#include "levels/ttm/areas/1/14/model.inc.c"
#include "levels/ttm/areas/1/15/model.inc.c"
#include "levels/ttm/areas/1/16/1.inc.c"
#include "levels/ttm/areas/1/16/2.inc.c"
#include "levels/ttm/areas/1/17/1.inc.c"
#include "levels/ttm/areas/1/17/2.inc.c"
#include "levels/ttm/areas/1/18/1.inc.c"
#include "levels/ttm/areas/1/18/2.inc.c"
#include "levels/ttm/areas/1/19/model.inc.c"
#include "levels/ttm/areas/1/20/model.inc.c"
#include "levels/ttm/areas/1/21/1.inc.c"
#include "levels/ttm/areas/1/21/2.inc.c"
#include "levels/ttm/rolling_log/model.inc.c"
#include "levels/ttm/areas/1/painting.inc.c"
#include "levels/ttm/star_cage/1.inc.c"
#include "levels/ttm/star_cage/2.inc.c"
#include "levels/ttm/rolling_log/collision.inc.c"
#include "levels/ttm/star_cage/collision.inc.c"
#include "levels/ttm/areas/1/collision.inc.c"
#include "levels/ttm/areas/1/macro.inc.c"
#include "levels/ttm/areas/1/trajectory.inc.c"
#include "levels/ttm/areas/1/movtext.inc.c"
#include "levels/ttm/areas/2/1/model.inc.c"
#include "levels/ttm/areas/2/2/model.inc.c"
#include "levels/ttm/areas/3/1/model.inc.c"
#include "levels/ttm/areas/3/2/model.inc.c"
#include "levels/ttm/areas/3/3/model.inc.c"
#include "levels/ttm/areas/4/1/model.inc.c"
#include "levels/ttm/areas/4/2/model.inc.c"
#include "levels/ttm/blue_smiley/model.inc.c"
#include "levels/ttm/yellow_smiley/model.inc.c"
#include "levels/ttm/star_smiley/model.inc.c"
#include "levels/ttm/moon_smiley/model.inc.c"
#include "levels/ttm/slide_exit_podium/model.inc.c"
#include "levels/ttm/areas/2/collision.inc.c"
#include "levels/ttm/areas/3/collision.inc.c"
#include "levels/ttm/areas/4/collision.inc.c"
#include "levels/ttm/slide_exit_podium/collision.inc.c"
#include "levels/ttm/areas/2/macro.inc.c"
#include "levels/ttm/areas/3/macro.inc.c"
#include "levels/ttm/areas/4/macro.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(ttm_seg7_dl_070090D0);
	EXPORT_DL(ttm_seg7_dl_07009C98);
	EXPORT_DL(ttm_seg7_dl_07009CE0);
	EXPORT_DL(ttm_seg7_dl_0700A038);
	EXPORT_DL(ttm_seg7_dl_0700A098);
	EXPORT_DL(ttm_seg7_dl_0700A120);
	EXPORT_DL(ttm_seg7_dl_0700A268);
	EXPORT_DL(ttm_seg7_dl_0700A2E0);
	EXPORT_DL(ttm_seg7_dl_0700A710);
	EXPORT_DL(ttm_seg7_dl_0700A848);
	EXPORT_DL(ttm_seg7_dl_0700A8C0);
	EXPORT_DL(ttm_seg7_dl_0700AA40);
	EXPORT_DL(ttm_seg7_dl_0700AAB8);
	EXPORT_DL(ttm_seg7_dl_0700AB08);
	EXPORT_DL(ttm_seg7_dl_0700AE58);
	EXPORT_DL(ttm_seg7_dl_0700AF90);
	EXPORT_DL(ttm_seg7_dl_0700B068);
	EXPORT_DL(ttm_seg7_dl_0700B0D0);
	EXPORT_DL(ttm_seg7_dl_0700B3F8);
	EXPORT_DL(ttm_seg7_dl_0700B530);
	EXPORT_DL(ttm_seg7_dl_0700B608);
	EXPORT_DL(ttm_seg7_dl_0700B670);
	EXPORT_DL(ttm_seg7_dl_0700B998);
	EXPORT_DL(ttm_seg7_dl_0700BAD0);
	EXPORT_DL(ttm_seg7_dl_0700BBA8);
	EXPORT_DL(ttm_seg7_dl_0700BC10);
	EXPORT_DL(ttm_seg7_dl_0700BDA0);
	EXPORT_DL(ttm_seg7_dl_0700BE20);
	EXPORT_DL(ttm_seg7_dl_0700BFE0);
	EXPORT_DL(ttm_seg7_dl_0700C070);
	EXPORT_DL(ttm_seg7_dl_0700C2E8);
	EXPORT_DL(ttm_seg7_dl_0700C360);
	EXPORT_DL(ttm_seg7_dl_0700C3D8);
	EXPORT_DL(ttm_seg7_dl_0700C408);
	EXPORT_DL(ttm_seg7_dl_0700C900);
	EXPORT_DL(ttm_seg7_dl_0700C9F8);
	EXPORT_DL(ttm_seg7_dl_0700CA78);
	EXPORT_DL(ttm_seg7_dl_0700CAE0);
	EXPORT_DL(ttm_seg7_dl_0700CC60);
	EXPORT_DL(ttm_seg7_dl_0700CCC8);
	EXPORT_DL(ttm_seg7_dl_0700CD10);
	EXPORT_DL(ttm_seg7_dl_0700D090);
	EXPORT_DL(ttm_seg7_dl_0700D180);
	EXPORT_DL(ttm_seg7_dl_0700D1D8);
	EXPORT_DL(ttm_seg7_dl_0700D548);
	EXPORT_DL(ttm_seg7_dl_0700D640);
	EXPORT_DL(ttm_seg7_dl_0700D688);
	EXPORT_DL(ttm_seg7_dl_0700DDB8);
	EXPORT_DL(ttm_seg7_dl_0700DF78);
	EXPORT_DL(ttm_seg7_dl_0700E240);
	EXPORT_DL(ttm_seg7_dl_0700E308);
	EXPORT_DL(ttm_seg7_dl_0700E910);
	EXPORT_DL(ttm_seg7_dl_0700EA28);
	EXPORT_DL(ttm_seg7_dl_0700EAC8);
	EXPORT_DL(ttm_seg7_dl_0700EC00);
	EXPORT_DL(ttm_seg7_dl_0700EC58);
	EXPORT_DL(ttm_seg7_dl_0700F0D8);
	EXPORT_DL(ttm_seg7_dl_0700F170);
	EXPORT_DL(ttm_seg7_dl_0700F270);
	EXPORT_DL(ttm_seg7_dl_0700F858);
	EXPORT_DL(ttm_seg7_dl_0700F8D0);
	EXPORT_DL(ttm_seg7_dl_0700FA18);
	EXPORT_DL(ttm_seg7_dl_07010638);
	EXPORT_DL(ttm_seg7_dl_07010830);
	EXPORT_DL(ttm_seg7_dl_07010978);
	EXPORT_DL(ttm_seg7_dl_07010A30);
	EXPORT_DL(ttm_seg7_dl_07010A68);
	EXPORT_DL(ttm_seg7_dl_07010F90);
	EXPORT_DL(ttm_seg7_dl_07011078);
	EXPORT_DL(ttm_seg7_dl_07011128);
	EXPORT_DL(ttm_seg7_dl_070114B8);
	EXPORT_DL(ttm_seg7_dl_070115B0);
	EXPORT_DL(ttm_seg7_dl_07011608);
	EXPORT_DL(ttm_seg7_dl_07011AA8);
	EXPORT_DL(ttm_seg7_dl_07011B20);
	EXPORT_DL(ttm_seg7_dl_07011C10);
	EXPORT_DL(ttm_seg7_dl_07011C48);
	EXPORT_DL(ttm_seg7_dl_07011C78);
	EXPORT_DL(ttm_seg7_dl_07011D40);
	EXPORT_DL(ttm_seg7_dl_07011D78);
	EXPORT_DL(ttm_seg7_dl_07012120);
	EXPORT_DL(ttm_seg7_dl_070121B8);
	EXPORT_DL(ttm_seg7_dl_07012270);
	EXPORT_DL(ttm_seg7_dl_07012388);
	EXPORT_DL(ttm_seg7_dl_070123A0);
	EXPORT_DL(ttm_seg7_dl_070123B8);
	EXPORT_DL(ttm_seg7_dl_07012410);
	EXPORT_DL(ttm_seg7_painting_dl_07012430);
	EXPORT_DL(ttm_seg7_painting_dl_07012E98);
	EXPORT_DL(ttm_seg7_dl_07013308);
	EXPORT_DL(ttm_seg7_dl_07013430);
	EXPORT_DL(ttm_seg7_dl_070135A0);
	EXPORT_DL(ttm_seg7_dl_07013608);
	EXPORT_DL(ttm_dl_waterfall);
	EXPORT_DL(ttm_dl_bottom_waterfall);
	EXPORT_DL(ttm_dl_puddle_waterfall);
	EXPORT_DL(ttm_seg7_dl_0701BEE0);
	EXPORT_DL(ttm_seg7_dl_0701C008);
	EXPORT_DL(ttm_seg7_dl_0701C688);
	EXPORT_DL(ttm_seg7_dl_0701C6F8);
	EXPORT_DL(ttm_seg7_dl_0701D708);
	EXPORT_DL(ttm_seg7_dl_0701D798);
	EXPORT_DL(ttm_seg7_dl_0701DAE0);
	EXPORT_DL(ttm_seg7_dl_0701DBB8);
	EXPORT_DL(ttm_seg7_dl_07022F08);
	EXPORT_DL(ttm_seg7_dl_07023108);
	EXPORT_DL(ttm_seg7_dl_070240F0);
	EXPORT_DL(ttm_seg7_dl_07024178);
	EXPORT_DL(ttm_seg7_dl_07024708);
	EXPORT_DL(ttm_seg7_dl_070247F0);
	EXPORT_DL(ttm_seg7_dl_070249A0);
	EXPORT_DL(ttm_seg7_dl_07024C00);
	EXPORT_DL(ttm_seg7_dl_07024C78);
	EXPORT_DL(ttm_seg7_dl_07025B30);
	EXPORT_DL(ttm_seg7_dl_07025F48);
	EXPORT_DL(ttm_seg7_dl_070291B8);
	EXPORT_DL(ttm_seg7_dl_070299D0);
	EXPORT_DL(ttm_seg7_dl_07029A18);
	EXPORT_DL(ttm_seg7_dl_0702A0D8);
	EXPORT_DL(ttm_seg7_dl_0702A1B8);
	EXPORT_DL(ttm_seg7_dl_0702A738);
	EXPORT_DL(ttm_seg7_dl_0702A8A0);
	EXPORT_DL(ttm_seg7_dl_0702A988);
	EXPORT_DL(ttm_seg7_dl_0702A9C0);
	EXPORT_DL(ttm_seg7_dl_0702AA70);
	EXPORT_DL(ttm_seg7_dl_0702AAA8);
	EXPORT_DL(ttm_seg7_dl_0702AB58);
	EXPORT_DL(ttm_seg7_dl_0702AB90);
	EXPORT_DL(ttm_seg7_dl_0702AC40);
	EXPORT_DL(ttm_seg7_dl_0702AC78);
	EXPORT_DL(ttm_seg7_dl_0702B9D0);
	EXPORT_DL(ttm_seg7_dl_0702BB60);
}
