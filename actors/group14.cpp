#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"
#include "group14.h"

#include "koopa_flag/model.inc.c"
#include "koopa_flag/anims/data.inc.c"
#include "koopa_flag/anims/table.inc.c"
UNUSED static const u64 binid_0 = 0;

#include "poundable_pole/model.inc.c"
#include "poundable_pole/collision.inc.c"
UNUSED static const u64 binid_1 = 1;

#include "koopa/model.inc.c"
#include "koopa/anims/data.inc.c"
#include "koopa/anims/table.inc.c"
UNUSED static const u64 binid_2 = 2;

#include "piranha_plant/model.inc.c"
#include "piranha_plant/anims/data.inc.c"
#include "piranha_plant/anims/table.inc.c"
UNUSED static const u64 binid_3 = 3;

#include "whomp/model.inc.c"
#include "whomp/anims/data.inc.c"
#include "whomp/anims/table.inc.c"
#include "whomp/collision.inc.c"
UNUSED static const u64 binid_4 = 4;

#include "chain_ball/model.inc.c"
UNUSED static const u64 binid_5 = 5;

#include "chain_chomp/model.inc.c"
#include "chain_chomp/anims/data.inc.c"
#include "chain_chomp/anims/table.inc.c"
UNUSED static const u64 binid_6 = 6;

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(koopa_flag_seg6_dl_06000968);
	EXPORT_DL(koopa_flag_seg6_dl_06000A08);
	EXPORT_DL(koopa_flag_seg6_dl_06000AB8);
	EXPORT_DL(koopa_flag_seg6_dl_06000AE8);
	EXPORT_DL(koopa_flag_seg6_dl_06000B08);
	EXPORT_DL(koopa_flag_seg6_dl_06000BC0);
	EXPORT_DL(koopa_flag_seg6_dl_06000BF8);
	EXPORT_DL(koopa_flag_seg6_dl_06000CA8);
	EXPORT_DL(koopa_flag_seg6_dl_06000CE0);
	EXPORT_DL(koopa_flag_seg6_dl_06000D90);
	EXPORT_DL(koopa_flag_seg6_dl_06000DB0);
	EXPORT_DL(koopa_flag_seg6_dl_06000E20);
	EXPORT_DL(koopa_flag_seg6_dl_06000E38);
	EXPORT_DL(poundable_pole_seg6_dl_060022F0);
	EXPORT_DL(poundable_pole_seg6_dl_06002358);
	EXPORT_DL(poundable_pole_seg6_dl_06002410);
	EXPORT_DL(koopa_seg6_dl_06007658);
	EXPORT_DL(koopa_seg6_dl_060076B0);
	EXPORT_DL(koopa_seg6_dl_06007850);
	EXPORT_DL(koopa_seg6_dl_06007970);
	EXPORT_DL(koopa_seg6_dl_06007A60);
	EXPORT_DL(koopa_seg6_dl_06007AA0);
	EXPORT_DL(koopa_seg6_dl_06007AF8);
	EXPORT_DL(koopa_seg6_dl_06007B20);
	EXPORT_DL(koopa_seg6_dl_06007FB8);
	EXPORT_DL(koopa_seg6_dl_06008050);
	EXPORT_DL(koopa_seg6_dl_060080F8);
	EXPORT_DL(koopa_seg6_dl_06008250);
	EXPORT_DL(koopa_seg6_dl_06008530);
	EXPORT_DL(koopa_seg6_dl_060085E8);
	EXPORT_DL(koopa_seg6_dl_06008640);
	EXPORT_DL(koopa_seg6_dl_060086D0);
	EXPORT_DL(koopa_seg6_dl_060089B0);
	EXPORT_DL(koopa_seg6_dl_06008A68);
	EXPORT_DL(koopa_seg6_dl_06008AC0);
	EXPORT_DL(koopa_seg6_dl_06008B50);
	EXPORT_DL(koopa_seg6_dl_0600B6E0);
	EXPORT_DL(koopa_seg6_dl_0600B838);
	EXPORT_DL(koopa_seg6_dl_0600B8B8);
	EXPORT_DL(koopa_seg6_dl_0600BA10);
	EXPORT_DL(koopa_seg6_dl_0600BA90);
	EXPORT_DL(koopa_seg6_dl_0600BC18);
	EXPORT_DL(koopa_seg6_dl_0600BC88);
	EXPORT_DL(koopa_seg6_dl_0600BE10);
	EXPORT_DL(koopa_seg6_dl_0600BE80);
	EXPORT_DL(koopa_seg6_dl_0600C240);
	EXPORT_DL(koopa_seg6_dl_0600C498);
	EXPORT_DL(piranha_plant_seg6_dl_06015438);
	EXPORT_DL(piranha_plant_seg6_dl_06015480);
	EXPORT_DL(piranha_plant_seg6_dl_06015530);
	EXPORT_DL(piranha_plant_seg6_dl_06015578);
	EXPORT_DL(piranha_plant_seg6_dl_06015798);
	EXPORT_DL(piranha_plant_seg6_dl_06015850);
	EXPORT_DL(piranha_plant_seg6_dl_06015E40);
	EXPORT_DL(piranha_plant_seg6_dl_06015EA8);
	EXPORT_DL(piranha_plant_seg6_dl_06015F68);
	EXPORT_DL(piranha_plant_seg6_dl_06016060);
	EXPORT_DL(piranha_plant_seg6_dl_060160B0);
	EXPORT_DL(piranha_plant_seg6_dl_06016120);
	EXPORT_DL(piranha_plant_seg6_dl_06016750);
	EXPORT_DL(piranha_plant_seg6_dl_060167B8);
	EXPORT_DL(piranha_plant_seg6_dl_06016890);
	EXPORT_DL(piranha_plant_seg6_dl_06016960);
	EXPORT_DL(piranha_plant_seg6_dl_060169A8);
	EXPORT_DL(piranha_plant_seg6_dl_060169E8);
	EXPORT_DL(piranha_plant_seg6_dl_06016A78);
	EXPORT_DL(piranha_plant_seg6_dl_06016BA8);
	EXPORT_DL(piranha_plant_seg6_dl_06016C10);
	EXPORT_DL(piranha_plant_seg6_dl_06016D30);
	EXPORT_DL(piranha_plant_seg6_dl_06016D98);
	EXPORT_DL(piranha_plant_seg6_dl_06016EB8);
	EXPORT_DL(piranha_plant_seg6_dl_06016F20);
	EXPORT_DL(whomp_seg6_dl_0601F4F8);
	EXPORT_DL(whomp_seg6_dl_0601F570);
	EXPORT_DL(whomp_seg6_dl_0601F5A8);
	EXPORT_DL(whomp_seg6_dl_0601F5E0);
	EXPORT_DL(whomp_seg6_dl_0601F7F0);
	EXPORT_DL(whomp_seg6_dl_0601F880);
	EXPORT_DL(whomp_seg6_dl_0601FA58);
	EXPORT_DL(whomp_seg6_dl_0601FAE8);
	EXPORT_DL(whomp_seg6_dl_0601FB88);
	EXPORT_DL(whomp_seg6_dl_0601FBC0);
	EXPORT_DL(whomp_seg6_dl_0601FC70);
	EXPORT_DL(whomp_seg6_dl_0601FCA8);
	EXPORT_DL(whomp_seg6_dl_0601FE10);
	EXPORT_DL(whomp_seg6_dl_0601FEA8);
	EXPORT_DL(whomp_seg6_dl_0601FFA0);
	EXPORT_DL(whomp_seg6_dl_06020038);
	EXPORT_DL(chain_ball_seg6_dl_060212E8);
	EXPORT_DL(chain_chomp_seg6_dl_06024000);
	EXPORT_DL(chain_chomp_seg6_dl_06024040);
	EXPORT_DL(chain_chomp_seg6_dl_06024240);
	EXPORT_DL(chain_chomp_seg6_dl_06024700);
	EXPORT_DL(chain_chomp_seg6_dl_06024900);
	EXPORT_DL(chain_chomp_seg6_dl_06024940);
	EXPORT_DL(chain_chomp_seg6_dl_06024AA8);
	EXPORT_DL(chain_chomp_seg6_dl_06024B00);
	EXPORT_DL(chain_chomp_seg6_dl_06024CF0);
	EXPORT_DL(chain_chomp_seg6_dl_06024D60);
	EXPORT_DL(chain_chomp_seg6_dl_06024F50);
	EXPORT_DL(chain_chomp_seg6_dl_06024FC0);
}
