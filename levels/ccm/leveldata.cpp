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
#include "levels/ccm/texture.inc.c"
#include "levels/ccm/areas/1/1/model.inc.c"
#include "levels/ccm/areas/1/2/model.inc.c"
#include "levels/ccm/areas/1/3/model.inc.c"
#include "levels/ccm/areas/1/4/model.inc.c"
#include "levels/ccm/areas/1/5/model.inc.c"
#include "levels/ccm/areas/1/6/1.inc.c"
#include "levels/ccm/areas/1/6/2.inc.c"
#include "levels/ccm/areas/1/7/1.inc.c"
#include "levels/ccm/areas/1/7/2.inc.c"
#include "levels/ccm/areas/1/7/3.inc.c"
#include "levels/ccm/areas/1/8/model.inc.c"
#include "levels/ccm/areas/1/9/model.inc.c"
#include "levels/ccm/areas/1/10/1.inc.c"
#include "levels/ccm/areas/1/10/2.inc.c"
#include "levels/ccm/areas/1/10/3.inc.c"
#include "levels/ccm/ropeway_lift/1.inc.c"
#include "levels/ccm/ropeway_lift/2.inc.c"
#include "levels/ccm/snowman_base/model.inc.c"
#include "levels/ccm/snowman_head/1.inc.c"
#include "levels/ccm/snowman_head/2.inc.c"
#include "levels/ccm/areas/1/collision.inc.c"
#include "levels/ccm/areas/1/macro.inc.c"
#include "levels/ccm/ropeway_lift/collision.inc.c"
#include "levels/ccm/areas/1/trajectory.inc.c"
#include "levels/ccm/areas/1/movtext.inc.c"
#include "levels/ccm/areas/2/1/model.inc.c"
#include "levels/ccm/areas/2/2/model.inc.c"
#include "levels/ccm/areas/2/3/model.inc.c"
#include "levels/ccm/areas/2/4/model.inc.c"
#include "levels/ccm/areas/2/5/model.inc.c"
#include "levels/ccm/areas/2/6/model.inc.c"
#include "levels/ccm/areas/2/7/model.inc.c"
#include "levels/ccm/areas/2/collision.inc.c"
#include "levels/ccm/areas/2/macro.inc.c"
#include "levels/ccm/areas/2/trajectory.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(ccm_seg7_dl_07009820);
	EXPORT_DL(ccm_seg7_dl_07009DD0);
	EXPORT_DL(ccm_seg7_dl_07009F28);
	EXPORT_DL(ccm_seg7_dl_0700A248);
	EXPORT_DL(ccm_seg7_dl_0700AB00);
	EXPORT_DL(ccm_seg7_dl_0700ABF8);
	EXPORT_DL(ccm_seg7_dl_0700B090);
	EXPORT_DL(ccm_seg7_dl_0700B190);
	EXPORT_DL(ccm_seg7_dl_0700B1D8);
	EXPORT_DL(ccm_seg7_dl_0700BF88);
	EXPORT_DL(ccm_seg7_dl_0700C000);
	EXPORT_DL(ccm_seg7_dl_0700C220);
	EXPORT_DL(ccm_seg7_dl_0700C330);
	EXPORT_DL(ccm_seg7_dl_0700C380);
	EXPORT_DL(ccm_seg7_dl_0700D128);
	EXPORT_DL(ccm_seg7_dl_0700D1E0);
	EXPORT_DL(ccm_seg7_dl_0700D2A8);
	EXPORT_DL(ccm_seg7_dl_0700D348);
	EXPORT_DL(ccm_seg7_dl_0700D4D8);
	EXPORT_DL(ccm_seg7_dl_0700D540);
	EXPORT_DL(ccm_seg7_dl_0700D578);
	EXPORT_DL(ccm_seg7_dl_0700DC18);
	EXPORT_DL(ccm_seg7_dl_0700DD28);
	EXPORT_DL(ccm_seg7_dl_0700DDF0);
	EXPORT_DL(ccm_seg7_dl_0700E4A8);
	EXPORT_DL(ccm_seg7_dl_0700E530);
	EXPORT_DL(ccm_seg7_dl_0700E668);
	EXPORT_DL(ccm_seg7_dl_0700E6C0);
	EXPORT_DL(ccm_seg7_dl_0700E708);
	EXPORT_DL(ccm_seg7_dl_0700E8D0);
	EXPORT_DL(ccm_seg7_dl_0700E928);
	EXPORT_DL(ccm_seg7_dl_0700E970);
	EXPORT_DL(ccm_seg7_dl_0700F1B8);
	EXPORT_DL(ccm_seg7_dl_0700F210);
	EXPORT_DL(ccm_seg7_dl_0700F3B0);
	EXPORT_DL(ccm_seg7_dl_0700F440);
	EXPORT_DL(ccm_seg7_dl_0700F5C0);
	EXPORT_DL(ccm_seg7_dl_0700F5F8);
	EXPORT_DL(ccm_seg7_dl_0700F650);
	EXPORT_DL(ccm_seg7_dl_0700F738);
	EXPORT_DL(ccm_seg7_dl_0700F780);
	EXPORT_DL(ccm_seg7_dl_0700FA18);
	EXPORT_DL(ccm_seg7_dl_0700FA70);
	EXPORT_DL(ccm_seg7_dl_0700FB00);
	EXPORT_DL(ccm_seg7_dl_0700FC90);
	EXPORT_DL(ccm_seg7_dl_0700FD08);
	EXPORT_DL(ccm_seg7_dl_07010390);
	EXPORT_DL(ccm_seg7_dl_07010480);
	EXPORT_DL(ccm_seg7_dl_07010518);
	EXPORT_DL(ccm_seg7_dl_07010610);
	EXPORT_DL(ccm_seg7_dl_07010660);
	EXPORT_DL(ccm_seg7_dl_070108F8);
	EXPORT_DL(ccm_seg7_dl_07010940);
	EXPORT_DL(ccm_seg7_dl_070109D0);
	EXPORT_DL(ccm_seg7_dl_07010AF8);
	EXPORT_DL(ccm_seg7_dl_07010B50);
	EXPORT_DL(ccm_seg7_dl_07010E30);
	EXPORT_DL(ccm_seg7_dl_07010EC0);
	EXPORT_DL(ccm_seg7_dl_07010F28);
	EXPORT_DL(ccm_seg7_dl_07011660);
	EXPORT_DL(ccm_seg7_dl_070117B8);
	EXPORT_DL(ccm_seg7_dl_070118B0);
	EXPORT_DL(ccm_seg7_dl_070128E8);
	EXPORT_DL(ccm_seg7_dl_07012BD8);
	EXPORT_DL(ccm_seg7_dl_070133E0);
	EXPORT_DL(ccm_seg7_dl_070136D0);
	EXPORT_DL(ccm_seg7_dl_070137F0);
	EXPORT_DL(ccm_seg7_dl_07013838);
	EXPORT_DL(ccm_seg7_dl_07013870);
	EXPORT_DL(ccm_seg7_dl_0701B0E0);
	EXPORT_DL(ccm_seg7_dl_0701B128);
	EXPORT_DL(ccm_seg7_dl_0701C5C8);
	EXPORT_DL(ccm_seg7_dl_0701CC40);
	EXPORT_DL(ccm_seg7_dl_0701CE30);
	EXPORT_DL(ccm_seg7_dl_0701DF68);
	EXPORT_DL(ccm_seg7_dl_0701E3F8);
	EXPORT_DL(ccm_seg7_dl_0701E4E8);
	EXPORT_DL(ccm_seg7_dl_0701E520);
	EXPORT_DL(ccm_seg7_dl_0701E558);
	EXPORT_DL(ccm_seg7_dl_0701E668);
	EXPORT_DL(ccm_seg7_dl_0701E6B0);
	EXPORT_DL(ccm_seg7_dl_0701F7C8);
	EXPORT_DL(ccm_seg7_dl_0701FC78);
	EXPORT_DL(ccm_seg7_dl_0701FD30);
	EXPORT_DL(ccm_seg7_dl_0701FD78);
	EXPORT_DL(ccm_seg7_dl_0701FE28);
	EXPORT_DL(ccm_seg7_dl_0701FE60);
	EXPORT_DL(ccm_seg7_dl_07020590);
	EXPORT_DL(ccm_seg7_dl_070205E8);
	EXPORT_DL(ccm_seg7_dl_070207F0);
}
