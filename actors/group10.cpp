#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"
#include "group10.h"

#include "bird/model.inc.c"
#include "bird/anims/data.inc.c"
#include "bird/anims/table.inc.c"
UNUSED static const u64 binid_0 = 0;

#include "peach/model.inc.c"
#include "peach/anims/data.inc.c"
#include "peach/anims/table.inc.c"
UNUSED static const u64 binid_1 = 1;

#include "yoshi/model.inc.c"
#include "yoshi/anims/data.inc.c"
#include "yoshi/anims/table.inc.c"
UNUSED static const u64 binid_2 = 2;

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(birds_seg5_dl_05000528);
	EXPORT_DL(birds_seg5_dl_05000598);
	EXPORT_DL(birds_seg5_dl_05000600);
	EXPORT_DL(birds_seg5_dl_05000670);
	EXPORT_DL(peach_seg5_dl_05005648);
	EXPORT_DL(peach_seg5_dl_05005750);
	EXPORT_DL(peach_seg5_dl_05005780);
	EXPORT_DL(peach_seg5_dl_050058B8);
	EXPORT_DL(peach_seg5_dl_050058E0);
	EXPORT_DL(peach_seg5_dl_05005C48);
	EXPORT_DL(peach_seg5_dl_05005CB0);
	EXPORT_DL(peach_seg5_dl_05005CE0);
	EXPORT_DL(peach_seg5_dl_05005D38);
	EXPORT_DL(peach_seg5_dl_05005D90);
	EXPORT_DL(peach_seg5_dl_05005DE8);
	EXPORT_DL(peach_seg5_dl_05005E40);
	EXPORT_DL(peach_seg5_dl_05005E98);
	EXPORT_DL(peach_seg5_dl_05005EF0);
	EXPORT_DL(peach_seg5_dl_05005F48);
	EXPORT_DL(peach_seg5_dl_05006058);
	EXPORT_DL(peach_seg5_dl_050060E0);
	EXPORT_DL(peach_seg5_dl_05006728);
	EXPORT_DL(peach_seg5_dl_05006798);
	EXPORT_DL(peach_seg5_dl_05006A18);
	EXPORT_DL(peach_seg5_dl_05006FA8);
	EXPORT_DL(peach_seg5_dl_05007230);
	EXPORT_DL(peach_seg5_us_dl_05007288);
	EXPORT_DL(peach_seg5_dl_05007810);
	EXPORT_DL(peach_seg5_dl_05007AB8);
	EXPORT_DL(peach_seg5_dl_05007CE8);
	EXPORT_DL(peach_seg5_dl_05007E58);
	EXPORT_DL(peach_seg5_dl_050080B8);
	EXPORT_DL(peach_seg5_dl_05008228);
	EXPORT_DL(peach_seg5_dl_05008428);
	EXPORT_DL(peach_seg5_dl_050084C0);
	EXPORT_DL(peach_seg5_dl_05008560);
	EXPORT_DL(peach_seg5_dl_050086C8);
	EXPORT_DL(peach_seg5_dl_050087A8);
	EXPORT_DL(peach_seg5_dl_05008978);
	EXPORT_DL(peach_seg5_dl_05008A10);
	EXPORT_DL(peach_seg5_dl_05008AB0);
	EXPORT_DL(peach_seg5_dl_05008C18);
	EXPORT_DL(peach_seg5_dl_05008CF8);
	EXPORT_DL(peach_seg5_dl_05008E38);
	EXPORT_DL(peach_seg5_dl_05008F20);
	EXPORT_DL(peach_seg5_dl_05009060);
	EXPORT_DL(peach_seg5_dl_05009148);
	EXPORT_DL(peach_seg5_dl_05009388);
	EXPORT_DL(peach_seg5_dl_05009500);
	EXPORT_DL(peach_seg5_dl_050097C0);
	EXPORT_DL(peach_seg5_dl_05009940);
	EXPORT_DL(peach_seg5_dl_05009E20);
	EXPORT_DL(peach_seg5_dl_0500A0A8);
	EXPORT_DL(peach_seg5_us_dl_0500A188);
	EXPORT_DL(yoshi_seg5_dl_0501D440);
	EXPORT_DL(yoshi_seg5_dl_0501D488);
	EXPORT_DL(yoshi_seg5_dl_0501D4E0);
	EXPORT_DL(yoshi_seg5_dl_0501D8F8);
	EXPORT_DL(yoshi_seg5_dl_0501D940);
	EXPORT_DL(yoshi_seg5_dl_0501D970);
	EXPORT_DL(yoshi_seg5_dl_0501D998);
	EXPORT_DL(yoshi_seg5_dl_050212B0);
	EXPORT_DL(yoshi_seg5_dl_05021490);
	EXPORT_DL(yoshi_seg5_dl_050215D8);
	EXPORT_DL(yoshi_seg5_dl_05021678);
	EXPORT_DL(yoshi_seg5_dl_05021718);
	EXPORT_DL(yoshi_seg5_dl_05021860);
	EXPORT_DL(yoshi_seg5_dl_05021900);
	EXPORT_DL(yoshi_seg5_dl_050219A0);
	EXPORT_DL(yoshi_seg5_dl_05021B90);
	EXPORT_DL(yoshi_seg5_dl_05021CD8);
	EXPORT_DL(yoshi_seg5_dl_05021E38);
	EXPORT_DL(yoshi_seg5_dl_05021F70);
	EXPORT_DL(yoshi_seg5_dl_05022160);
	EXPORT_DL(yoshi_seg5_dl_050222A8);
	EXPORT_DL(yoshi_seg5_dl_05022408);
	EXPORT_DL(yoshi_seg5_dl_05022608);
}
