#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"
#include "group13.h"

#include "skeeter/model.inc.c"
#include "skeeter/anims/data.inc.c"
#include "skeeter/anims/table.inc.c"
UNUSED static const u64 binid_0 = 0;

#include "seaweed/model.inc.c"
#include "seaweed/anims/data.inc.c"
#include "seaweed/anims/table.inc.c"
UNUSED static const u64 binid_1 = 1;

#include "water_mine/model.inc.c"
UNUSED static const u64 binid_2 = 2;

#include "cyan_fish/model.inc.c"
#include "cyan_fish/anims/data.inc.c"
#include "cyan_fish/anims/table.inc.c"
UNUSED static const u64 binid_3 = 3;

#include "bub/model.inc.c"
#include "bub/anims/data.inc.c"
#include "bub/anims/table.inc.c"
UNUSED static const u64 binid_4 = 4;

#include "water_ring/model.inc.c"
#include "water_ring/anims/data.inc.c"
#include "water_ring/anims/table.inc.c"
UNUSED static const u64 binid_5 = 5;

#include "treasure_chest/model.inc.c"
UNUSED static const u64 binid_6 = 6;

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(skeeter_seg6_dl_060009D0);
	EXPORT_DL(skeeter_seg6_dl_06000A08);
	EXPORT_DL(skeeter_seg6_dl_06000AB8);
	EXPORT_DL(skeeter_seg6_dl_06000AF0);
	EXPORT_DL(skeeter_seg6_dl_06000BA0);
	EXPORT_DL(skeeter_seg6_dl_06000BD8);
	EXPORT_DL(skeeter_seg6_dl_06000C78);
	EXPORT_DL(skeeter_seg6_dl_06000CA8);
	EXPORT_DL(skeeter_seg6_dl_06000D48);
	EXPORT_DL(skeeter_seg6_dl_06000D78);
	EXPORT_DL(skeeter_seg6_dl_06000E60);
	EXPORT_DL(skeeter_seg6_dl_06000EC0);
	EXPORT_DL(skeeter_seg6_dl_06003FF0);
	EXPORT_DL(skeeter_seg6_dl_06004040);
	EXPORT_DL(skeeter_seg6_dl_06004070);
	EXPORT_DL(skeeter_seg6_dl_060040A0);
	EXPORT_DL(skeeter_seg6_dl_060040F0);
	EXPORT_DL(skeeter_seg6_dl_06004120);
	EXPORT_DL(skeeter_seg6_dl_06004150);
	EXPORT_DL(skeeter_seg6_dl_060041A0);
	EXPORT_DL(skeeter_seg6_dl_060041D0);
	EXPORT_DL(skeeter_seg6_dl_06004200);
	EXPORT_DL(skeeter_seg6_dl_060045C8);
	EXPORT_DL(skeeter_seg6_dl_06004618);
	EXPORT_DL(skeeter_seg6_dl_06004648);
	EXPORT_DL(skeeter_seg6_dl_06004678);
	EXPORT_DL(skeeter_seg6_dl_06004A40);
	EXPORT_DL(skeeter_seg6_dl_06005328);
	EXPORT_DL(skeeter_seg6_dl_06005358);
	EXPORT_DL(seaweed_seg6_dl_06009E50);
	EXPORT_DL(seaweed_seg6_dl_06009E98);
	EXPORT_DL(seaweed_seg6_dl_06009F48);
	EXPORT_DL(seaweed_seg6_dl_06009F90);
	EXPORT_DL(seaweed_seg6_dl_0600A040);
	EXPORT_DL(seaweed_seg6_dl_0600A088);
	EXPORT_DL(seaweed_seg6_dl_0600A138);
	EXPORT_DL(seaweed_seg6_dl_0600A180);
	EXPORT_DL(water_mine_seg6_dl_0600D1F8);
	EXPORT_DL(water_mine_seg6_dl_0600D230);
	EXPORT_DL(water_mine_seg6_dl_0600D268);
	EXPORT_DL(water_mine_seg6_dl_0600D2E0);
	EXPORT_DL(water_mine_seg6_dl_0600D3F8);
	EXPORT_DL(cyan_fish_seg6_dl_0600DD20);
	EXPORT_DL(cyan_fish_seg6_dl_0600DDD8);
	EXPORT_DL(cyan_fish_seg6_dl_0600DE90);
	EXPORT_DL(cyan_fish_seg6_dl_0600DED8);
	EXPORT_DL(cyan_fish_seg6_dl_0600DFC0);
	EXPORT_DL(cyan_fish_seg6_dl_0600E038);
	EXPORT_DL(bub_seg6_dl_06011848);
	EXPORT_DL(bub_seg6_dl_060118C0);
	EXPORT_DL(bub_seg6_dl_06011918);
	EXPORT_DL(bub_seg6_dl_06011968);
	EXPORT_DL(bub_seg6_dl_06011A50);
	EXPORT_DL(bub_seg6_dl_06011B28);
	EXPORT_DL(bub_seg6_dl_06011C58);
	EXPORT_DL(bub_seg6_dl_06011CF0);
	EXPORT_DL(bub_seg6_dl_06011DC0);
	EXPORT_DL(bub_seg6_dl_06011E48);
	EXPORT_DL(bub_seg6_dl_06011F18);
	EXPORT_DL(bub_seg6_dl_06011FA0);
	EXPORT_DL(water_ring_seg6_dl_06013AC0);
	EXPORT_DL(treasure_chest_seg6_dl_06016D58);
	EXPORT_DL(treasure_chest_seg6_dl_06016DA0);
	EXPORT_DL(treasure_chest_seg6_dl_06016E18);
	EXPORT_DL(treasure_chest_seg6_dl_06016EE0);
	EXPORT_DL(treasure_chest_seg6_dl_06016F90);
	EXPORT_DL(treasure_chest_seg6_dl_06017680);
	EXPORT_DL(treasure_chest_seg6_dl_06017790);
	EXPORT_DL(treasure_chest_seg6_dl_06017810);
	EXPORT_DL(treasure_chest_seg6_dl_060178C0);
}
