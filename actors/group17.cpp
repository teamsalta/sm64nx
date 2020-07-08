#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"
#include "group17.h"

#include "mr_i_eyeball/model.inc.c"
UNUSED static const u64 binid_0 = 0;

#include "mr_i_iris/model.inc.c"
UNUSED static const u64 binid_1 = 1;

#include "swoop/model.inc.c"
#include "swoop/anims/data.inc.c"
#include "swoop/anims/table.inc.c"
UNUSED static const u64 binid_2 = 2;

#include "snufit/model.inc.c"
UNUSED static const u64 binid_3 = 3;

#include "dorrie/model.inc.c"
#include "dorrie/anims/data.inc.c"
#include "dorrie/anims/table.inc.c"
#include "dorrie/collision.inc.c"
UNUSED static const u64 binid_4 = 4;

#include "scuttlebug/model.inc.c"
#include "scuttlebug/anims/data.inc.c"
#include "scuttlebug/anims/table.inc.c"
UNUSED static const u64 binid_5 = 5;

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(mr_i_eyeball_seg6_dl_06002080);
	EXPORT_DL(mr_i_iris_seg6_dl_06004170);
	EXPORT_DL(mr_i_iris_seg6_dl_060041D8);
	EXPORT_DL(mr_i_iris_seg6_dl_060041F0);
	EXPORT_DL(mr_i_iris_seg6_dl_06004208);
	EXPORT_DL(mr_i_iris_seg6_dl_06004220);
	EXPORT_DL(swoop_seg6_dl_060065B8);
	EXPORT_DL(swoop_seg6_dl_060066F8);
	EXPORT_DL(swoop_seg6_dl_06006758);
	EXPORT_DL(swoop_seg6_dl_06006848);
	EXPORT_DL(swoop_seg6_dl_06006880);
	EXPORT_DL(swoop_seg6_dl_06006A10);
	EXPORT_DL(swoop_seg6_dl_06006A68);
	EXPORT_DL(swoop_seg6_dl_06006A88);
	EXPORT_DL(swoop_seg6_dl_06006B98);
	EXPORT_DL(swoop_seg6_dl_06006BD0);
	EXPORT_DL(swoop_seg6_dl_06006CC8);
	EXPORT_DL(swoop_seg6_dl_06006D00);
	EXPORT_DL(snufit_seg6_dl_060091E0);
	EXPORT_DL(snufit_seg6_dl_06009278);
	EXPORT_DL(snufit_seg6_dl_060092F0);
	EXPORT_DL(snufit_seg6_dl_06009498);
	EXPORT_DL(snufit_seg6_dl_06009668);
	EXPORT_DL(snufit_seg6_dl_06009700);
	EXPORT_DL(snufit_seg6_dl_06009748);
	EXPORT_DL(snufit_seg6_dl_060098A0);
	EXPORT_DL(snufit_seg6_dl_06009938);
	EXPORT_DL(snufit_seg6_dl_060099D8);
	EXPORT_DL(snufit_seg6_dl_06009A10);
	EXPORT_DL(snufit_seg6_dl_06009B18);
	EXPORT_DL(snufit_seg6_dl_06009B68);
	EXPORT_DL(dorrie_seg6_dl_0600B8E8);
	EXPORT_DL(dorrie_seg6_dl_0600B940);
	EXPORT_DL(dorrie_seg6_dl_0600BAF8);
	EXPORT_DL(dorrie_seg6_dl_0600BE68);
	EXPORT_DL(dorrie_seg6_dl_0600BF10);
	EXPORT_DL(dorrie_seg6_dl_0600C030);
	EXPORT_DL(dorrie_seg6_dl_0600C310);
	EXPORT_DL(dorrie_seg6_dl_0600C468);
	EXPORT_DL(dorrie_seg6_dl_0600C758);
	EXPORT_DL(dorrie_seg6_dl_0600C8B8);
	EXPORT_DL(dorrie_seg6_dl_0600CDE8);
	EXPORT_DL(dorrie_seg6_dl_0600CFD0);
	EXPORT_DL(dorrie_seg6_dl_0600D300);
	EXPORT_DL(dorrie_seg6_dl_0600D440);
	EXPORT_DL(dorrie_seg6_dl_0600D620);
	EXPORT_DL(dorrie_seg6_dl_0600D6D8);
	EXPORT_DL(dorrie_seg6_dl_0600D818);
	EXPORT_DL(dorrie_seg6_dl_0600D8B0);
	EXPORT_DL(dorrie_seg6_dl_0600D9F0);
	EXPORT_DL(dorrie_seg6_dl_0600DA88);
	EXPORT_DL(dorrie_seg6_dl_0600DBC8);
	EXPORT_DL(dorrie_seg6_dl_0600DC60);
	EXPORT_DL(dorrie_seg6_dl_0600DDA0);
	EXPORT_DL(dorrie_seg6_dl_0600DE38);
	EXPORT_DL(dorrie_seg6_dl_0600DF60);
	EXPORT_DL(dorrie_seg6_dl_0600DFA8);
	EXPORT_DL(scuttlebug_seg6_dl_06013988);
	EXPORT_DL(scuttlebug_seg6_dl_060139C0);
	EXPORT_DL(scuttlebug_seg6_dl_060139F8);
	EXPORT_DL(scuttlebug_seg6_dl_06013AB0);
	EXPORT_DL(scuttlebug_seg6_dl_06013AE8);
	EXPORT_DL(scuttlebug_seg6_dl_06013B98);
	EXPORT_DL(scuttlebug_seg6_dl_06013BD0);
	EXPORT_DL(scuttlebug_seg6_dl_06013C80);
	EXPORT_DL(scuttlebug_seg6_dl_06013CB8);
	EXPORT_DL(scuttlebug_seg6_dl_06013D68);
	EXPORT_DL(scuttlebug_seg6_dl_06013DA0);
	EXPORT_DL(scuttlebug_seg6_dl_06013FC8);
	EXPORT_DL(scuttlebug_seg6_dl_060140F0);
	EXPORT_DL(scuttlebug_seg6_dl_06014150);
	EXPORT_DL(scuttlebug_seg6_dl_06014188);
	EXPORT_DL(scuttlebug_seg6_dl_06014238);
	EXPORT_DL(scuttlebug_seg6_dl_06014270);
	EXPORT_DL(scuttlebug_seg6_dl_06014338);
	EXPORT_DL(scuttlebug_seg6_dl_06014378);
}
