#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"
#include "group12.h"

#include "bowser_flame/model.inc.c"
UNUSED static const u64 binid_0 = 0;

#include "impact_ring/model.inc.c"
UNUSED static const u64 binid_1 = 1;

#include "yellow_sphere/model.inc.c"
UNUSED static const u64 binid_2 = 2;

#include "bowser/model.inc.c"
#include "bowser/anims/data.inc.c"
#include "bowser/anims/table.inc.c"
#include "bowser/flames_pos.inc.c"
UNUSED static const u64 binid_3 = 3;

#include "bomb/model.inc.c"
UNUSED static const u64 binid_4 = 4;

#include "impact_smoke/model.inc.c"
UNUSED static const u64 binid_5 = 5;

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(flame_seg6_dl_0601C080);
	EXPORT_DL(flame_seg6_dl_0601C0B0);
	EXPORT_DL(flame_seg6_dl_0601C0C8);
	EXPORT_DL(flame_seg6_dl_0601C0E0);
	EXPORT_DL(flame_seg6_dl_0601C108);
	EXPORT_DL(flame_seg6_dl_0601C1A8);
	EXPORT_DL(flame_seg6_dl_0601C248);
	EXPORT_DL(flame_seg6_dl_0601C2E8);
	EXPORT_DL(flame_seg6_dl_0601C388);
	EXPORT_DL(flame_seg6_dl_0601C428);
	EXPORT_DL(flame_seg6_dl_0601C4C8);
	EXPORT_DL(flame_seg6_dl_0601C568);
	EXPORT_DL(flame_seg6_dl_0601C608);
	EXPORT_DL(flame_seg6_dl_0601C6A8);
	EXPORT_DL(flame_seg6_dl_0601C748);
	EXPORT_DL(flame_seg6_dl_0601C7E8);
	EXPORT_DL(flame_seg6_dl_0601C888);
	EXPORT_DL(flame_seg6_dl_0601C928);
	EXPORT_DL(impact_ring_seg6_dl_0601EA50);
	EXPORT_DL(impact_ring_seg6_dl_0601EA88);
	EXPORT_DL(impact_ring_seg6_dl_0601EAC0);
	EXPORT_DL(yellow_sphere_seg6_dl_0601F388);
	EXPORT_DL(yellow_sphere_seg6_dl_0601F3C0);
	EXPORT_DL(bowser_seg6_dl_06039110);
	EXPORT_DL(bowser_seg6_dl_060391C8);
	EXPORT_DL(bowser_seg6_dl_06039260);
	EXPORT_DL(bowser_seg6_dl_060392E0);
	EXPORT_DL(bowser_seg6_dl_06039808);
	EXPORT_DL(bowser_seg6_dl_060398B0);
	EXPORT_DL(bowser_seg6_dl_06039958);
	EXPORT_DL(bowser_seg6_dl_060399D8);
	EXPORT_DL(bowser_seg6_dl_0603A210);
	EXPORT_DL(bowser_seg6_dl_0603A380);
	EXPORT_DL(bowser_seg6_dl_0603A4E8);
	EXPORT_DL(bowser_seg6_dl_0603AED8);
	EXPORT_DL(bowser_seg6_dl_0603B038);
	EXPORT_DL(bowser_seg6_dl_0603B208);
	EXPORT_DL(bowser_seg6_dl_0603B718);
	EXPORT_DL(bowser_seg6_dl_0603B8D0);
	EXPORT_DL(bowser_seg6_dl_0603B948);
	EXPORT_DL(bowser_seg6_dl_0603BDF8);
	EXPORT_DL(bowser_seg6_dl_0603BEA0);
	EXPORT_DL(bowser_seg6_dl_0603BF48);
	EXPORT_DL(bowser_seg6_dl_0603BFC8);
	EXPORT_DL(bowser_seg6_dl_0603C500);
	EXPORT_DL(bowser_seg6_dl_0603C5A8);
	EXPORT_DL(bowser_seg6_dl_0603C650);
	EXPORT_DL(bowser_seg6_dl_0603C6D0);
	EXPORT_DL(bowser_seg6_dl_0603CF28);
	EXPORT_DL(bowser_seg6_dl_0603D098);
	EXPORT_DL(bowser_seg6_dl_0603D200);
	EXPORT_DL(bowser_seg6_dl_0603DC70);
	EXPORT_DL(bowser_seg6_dl_0603DDD0);
	EXPORT_DL(bowser_seg6_dl_0603DFB0);
	EXPORT_DL(bowser_seg6_dl_0603E500);
	EXPORT_DL(bowser_seg6_dl_0603E6B8);
	EXPORT_DL(bowser_seg6_dl_0603FBA8);
	EXPORT_DL(bowser_seg6_dl_0603FC18);
	EXPORT_DL(bowser_seg6_dl_0603FD38);
	EXPORT_DL(bowser_seg6_dl_0603FDD0);
	EXPORT_DL(bowser_seg6_dl_0603FEA8);
	EXPORT_DL(bowser_seg6_dl_0603FFA8);
	EXPORT_DL(bowser_seg6_dl_06040180);
	EXPORT_DL(bowser_seg6_dl_060401F0);
	EXPORT_DL(bowser_seg6_dl_06040210);
	EXPORT_DL(bowser_seg6_dl_06040358);
	EXPORT_DL(bowser_seg6_dl_06041A28);
	EXPORT_DL(bowser_seg6_dl_06041BA0);
	EXPORT_DL(bowser_seg6_dl_06041E48);
	EXPORT_DL(bowser_seg6_dl_06041EA8);
	EXPORT_DL(bowser_seg6_dl_06041F50);
	EXPORT_DL(bowser_seg6_dl_06042098);
	EXPORT_DL(bowser_seg6_dl_060420F0);
	EXPORT_DL(bowser_seg6_dl_06042140);
	EXPORT_DL(bowser_seg6_dl_060421D8);
	EXPORT_DL(bowser_seg6_dl_06042220);
	EXPORT_DL(bowser_seg6_dl_06042278);
	EXPORT_DL(bowser_seg6_dl_06042328);
	EXPORT_DL(bowser_seg6_dl_06042348);
	EXPORT_DL(bowser_seg6_dl_060423A0);
	EXPORT_DL(bowser_seg6_dl_060423F8);
	EXPORT_DL(bowser_seg6_dl_06042450);
	EXPORT_DL(bowser_seg6_dl_060424A8);
	EXPORT_DL(bowser_seg6_dl_06042500);
	EXPORT_DL(bowser_seg6_dl_06042558);
	EXPORT_DL(bowser_seg6_dl_060425B0);
	EXPORT_DL(bowser_seg6_dl_06042608);
	EXPORT_DL(bowser_seg6_dl_06042EE0);
	EXPORT_DL(bowser_seg6_dl_060430E8);
	EXPORT_DL(bowser_seg6_dl_06043180);
	EXPORT_DL(bowser_seg6_dl_060431F0);
	EXPORT_DL(bowser_seg6_dl_06043548);
	EXPORT_DL(bowser_seg6_dl_06043648);
	EXPORT_DL(bowser_seg6_dl_06043698);
	EXPORT_DL(bomb_seg6_dl_0605A7C0);
	EXPORT_DL(bomb_seg6_dl_0605A7F8);
	EXPORT_DL(bomb_seg6_dl_0605A830);
	EXPORT_DL(bomb_seg6_dl_0605A8A8);
	EXPORT_DL(bomb_seg6_dl_0605A9C0);
	EXPORT_DL(impact_smoke_seg6_dl_06062AA8);
	EXPORT_DL(impact_smoke_seg6_dl_06062AD8);
	EXPORT_DL(impact_smoke_seg6_dl_06062AF0);
	EXPORT_DL(impact_smoke_seg6_dl_06062B08);
	EXPORT_DL(impact_smoke_seg6_dl_06062B38);
	EXPORT_DL(impact_smoke_seg6_dl_06062BD8);
	EXPORT_DL(impact_smoke_seg6_dl_06062C78);
	EXPORT_DL(impact_smoke_seg6_dl_06062D18);
}
