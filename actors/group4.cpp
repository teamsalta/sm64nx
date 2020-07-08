#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"
#include "group4.h"

#include "clam_shell/model.inc.c"
#include "clam_shell/anims/data.inc.c"
#include "clam_shell/anims/table.inc.c"
UNUSED static const u64 binid_0 = 0;

#include "manta/model.inc.c"
#include "manta/anims/data.inc.c"
#include "manta/geo.inc.c"
#include "manta/anims/table.inc.c"
UNUSED static const u64 binid_1 = 1;

#include "sushi/model.inc.c"
#include "sushi/anims/data.inc.c"
#include "sushi/anims/table.inc.c"
UNUSED static const u64 binid_2 = 2;

#include "unagi/model.inc.c"
#include "unagi/anims/data.inc.c"
#include "unagi/anims/table.inc.c"
UNUSED static const u64 binid_3 = 3;

#include "whirlpool/model.inc.c"
UNUSED static const u64 binid_4 = 4;

#include "game/debug.h"
namespace sm64
{
	EXPORT_GEO(manta_seg5_geo_05008D14);
	EXPORT_DL(clam_shell_seg5_dl_050011A8);
	EXPORT_DL(clam_shell_seg5_dl_05001238);
	EXPORT_DL(clam_shell_seg5_dl_050012B8);
	EXPORT_DL(clam_shell_seg5_dl_05001478);
	EXPORT_DL(clam_shell_seg5_dl_050014F8);
	EXPORT_DL(clam_shell_seg5_dl_05001568);
	EXPORT_DL(manta_seg5_dl_05004960);
	EXPORT_DL(manta_seg5_dl_050049C8);
	EXPORT_DL(manta_seg5_dl_05004A70);
	EXPORT_DL(manta_seg5_dl_05004CA8);
	EXPORT_DL(manta_seg5_dl_05004D10);
	EXPORT_DL(manta_seg5_dl_05004DB8);
	EXPORT_DL(manta_seg5_dl_05004E60);
	EXPORT_DL(manta_seg5_dl_05004E90);
	EXPORT_DL(manta_seg5_dl_05004FA0);
	EXPORT_DL(manta_seg5_dl_05005038);
	EXPORT_DL(manta_seg5_dl_05005248);
	EXPORT_DL(manta_seg5_dl_05005288);
	EXPORT_DL(manta_seg5_dl_05005308);
	EXPORT_DL(manta_seg5_dl_05005358);
	EXPORT_DL(manta_seg5_dl_050054F0);
	EXPORT_DL(manta_seg5_dl_05005558);
	EXPORT_DL(manta_seg5_dl_050055A8);
	EXPORT_DL(manta_seg5_dl_050056E0);
	EXPORT_DL(manta_seg5_dl_05005730);
	EXPORT_DL(manta_seg5_dl_05005768);
	EXPORT_DL(manta_seg5_dl_05005880);
	EXPORT_DL(manta_seg5_dl_05005918);
	EXPORT_DL(manta_seg5_dl_05005B28);
	EXPORT_DL(manta_seg5_dl_05005B68);
	EXPORT_DL(manta_seg5_dl_05005BE8);
	EXPORT_DL(manta_seg5_dl_05005C38);
	EXPORT_DL(manta_seg5_dl_05005DD0);
	EXPORT_DL(manta_seg5_dl_05005E38);
	EXPORT_DL(manta_seg5_dl_05005E88);
	EXPORT_DL(manta_seg5_dl_05005FC0);
	EXPORT_DL(manta_seg5_dl_05006010);
	EXPORT_DL(manta_seg5_dl_05006048);
	EXPORT_DL(manta_seg5_dl_05006510);
	EXPORT_DL(manta_seg5_dl_05006588);
	EXPORT_DL(manta_seg5_dl_050065F0);
	EXPORT_DL(manta_seg5_dl_050066D0);
	EXPORT_DL(manta_seg5_dl_05006750);
	EXPORT_DL(manta_seg5_dl_05006B08);
	EXPORT_DL(manta_seg5_dl_05006B70);
	EXPORT_DL(manta_seg5_dl_05006C08);
	EXPORT_DL(sushi_seg5_dl_05009CC0);
	EXPORT_DL(sushi_seg5_dl_05009DD0);
	EXPORT_DL(sushi_seg5_dl_05009F30);
	EXPORT_DL(sushi_seg5_dl_0500A008);
	EXPORT_DL(sushi_seg5_dl_0500A0E8);
	EXPORT_DL(sushi_seg5_dl_0500A160);
	EXPORT_DL(sushi_seg5_dl_0500A580);
	EXPORT_DL(sushi_seg5_dl_0500A748);
	EXPORT_DL(sushi_seg5_dl_0500A768);
	EXPORT_DL(sushi_seg5_dl_0500A860);
	EXPORT_DL(sushi_seg5_dl_0500A8A8);
	EXPORT_DL(sushi_seg5_dl_0500A958);
	EXPORT_DL(sushi_seg5_dl_0500A990);
	EXPORT_DL(unagi_seg5_dl_0500CCE0);
	EXPORT_DL(unagi_seg5_dl_0500CD30);
	EXPORT_DL(unagi_seg5_dl_0500CDD0);
	EXPORT_DL(unagi_seg5_dl_0500CF88);
	EXPORT_DL(unagi_seg5_dl_0500D038);
	EXPORT_DL(unagi_seg5_dl_0500D050);
	EXPORT_DL(unagi_seg5_dl_0500D1C0);
	EXPORT_DL(unagi_seg5_dl_0500D268);
	EXPORT_DL(unagi_seg5_dl_0500D3C0);
	EXPORT_DL(unagi_seg5_dl_0500D468);
	EXPORT_DL(unagi_seg5_dl_0500D5C0);
	EXPORT_DL(unagi_seg5_dl_0500D668);
	EXPORT_DL(unagi_seg5_dl_0500D7A0);
	EXPORT_DL(unagi_seg5_dl_0500D828);
	EXPORT_DL(unagi_seg5_dl_0500DB60);
	EXPORT_DL(unagi_seg5_dl_0500DBC0);
	EXPORT_DL(unagi_seg5_dl_0500DD08);
	EXPORT_DL(unagi_seg5_dl_0500DE70);
	EXPORT_DL(unagi_seg5_dl_0500DEB8);
	EXPORT_DL(unagi_seg5_dl_0500E030);
	EXPORT_DL(unagi_seg5_dl_0500E088);
	EXPORT_DL(unagi_seg5_dl_0500E200);
	EXPORT_DL(unagi_seg5_dl_0500E258);
	EXPORT_DL(whirlpool_seg5_dl_05013B58);
	EXPORT_DL(whirlpool_seg5_dl_05013CB8);
}
