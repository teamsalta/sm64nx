#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "moving_texture_macros.h"
#include "level_misc_macros.h"
#include "macro_preset_names.h"
#include "special_preset_names.h"
#include "textures.h"
#include "header.h"

#include "make_const_nonconst.h"
#include "levels/bits/texture.inc.c"
#include "levels/bits/areas/1/1/model.inc.c"
#include "levels/bits/areas/1/2/model.inc.c"
#include "levels/bits/areas/1/3/model.inc.c"
#include "levels/bits/areas/1/4/model.inc.c"
#include "levels/bits/areas/1/5/model.inc.c"
#include "levels/bits/areas/1/6/model.inc.c"
#include "levels/bits/areas/1/7/model.inc.c"
#include "levels/bits/areas/1/8/model.inc.c"
#include "levels/bits/areas/1/9/model.inc.c"
#include "levels/bits/areas/1/10/model.inc.c"
#include "levels/bits/areas/1/11/model.inc.c"
#include "levels/bits/areas/1/12/model.inc.c"
#include "levels/bits/areas/1/13/model.inc.c"
#include "levels/bits/areas/1/14/model.inc.c"
#include "levels/bits/areas/1/15/model.inc.c"
#include "levels/bits/areas/1/16/model.inc.c"
#include "levels/bits/areas/1/17/model.inc.c"
#include "levels/bits/areas/1/18/model.inc.c"
#include "levels/bits/areas/1/19/model.inc.c"
#include "levels/bits/areas/1/20/model.inc.c"
#include "levels/bits/areas/1/21/model.inc.c"
#include "levels/bits/areas/1/22/model.inc.c"
#include "levels/bits/areas/1/23/model.inc.c"
#include "levels/bits/areas/1/24/model.inc.c"
#include "levels/bits/areas/1/25/model.inc.c"
#include "levels/bits/areas/1/26/model.inc.c"
#include "levels/bits/areas/1/27/model.inc.c"
#include "levels/bits/areas/1/28/model.inc.c"
#include "levels/bits/areas/1/29/model.inc.c"
#include "levels/bits/areas/1/30/model.inc.c"
#include "levels/bits/areas/1/31/model.inc.c"
#include "levels/bits/areas/1/32/model.inc.c"
#include "levels/bits/areas/1/collision.inc.c"
#include "levels/bits/areas/1/macro.inc.c"
#include "levels/bits/areas/1/20/collision.inc.c"
#include "levels/bits/areas/1/21/collision.inc.c"
#include "levels/bits/areas/1/22/collision.inc.c"
#include "levels/bits/areas/1/23/collision.inc.c"
#include "levels/bits/areas/1/24/collision.inc.c"
#include "levels/bits/areas/1/25/collision.inc.c"
#include "levels/bits/areas/1/26/collision.inc.c"
#include "levels/bits/areas/1/27/collision.inc.c"
#include "levels/bits/areas/1/28/collision.inc.c"
#include "levels/bits/areas/1/29/collision.inc.c"
#include "levels/bits/areas/1/30/collision.inc.c"
#include "levels/bits/areas/1/31/collision.inc.c"
#include "levels/bits/areas/1/32/collision.inc.c"

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(bits_seg7_dl_070028C0);
	EXPORT_DL(bits_seg7_dl_07002918);
	EXPORT_DL(bits_seg7_dl_07003348);
	EXPORT_DL(bits_seg7_dl_070033B0);
	EXPORT_DL(bits_seg7_dl_070034F8);
	EXPORT_DL(bits_seg7_dl_070035E0);
	EXPORT_DL(bits_seg7_dl_07003670);
	EXPORT_DL(bits_seg7_dl_070043E8);
	EXPORT_DL(bits_seg7_dl_07004590);
	EXPORT_DL(bits_seg7_dl_070045F8);
	EXPORT_DL(bits_seg7_dl_07004720);
	EXPORT_DL(bits_seg7_dl_070047D0);
	EXPORT_DL(bits_seg7_dl_070047F0);
	EXPORT_DL(bits_seg7_dl_070058A0);
	EXPORT_DL(bits_seg7_dl_07005930);
	EXPORT_DL(bits_seg7_dl_07005C50);
	EXPORT_DL(bits_seg7_dl_07005D38);
	EXPORT_DL(bits_seg7_dl_07005DB8);
	EXPORT_DL(bits_seg7_dl_07007448);
	EXPORT_DL(bits_seg7_dl_07007AF0);
	EXPORT_DL(bits_seg7_dl_07007BE0);
	EXPORT_DL(bits_seg7_dl_07007C28);
	EXPORT_DL(bits_seg7_dl_07007E18);
	EXPORT_DL(bits_seg7_dl_07007E60);
	EXPORT_DL(bits_seg7_dl_07007EC8);
	EXPORT_DL(bits_seg7_dl_07008988);
	EXPORT_DL(bits_seg7_dl_07008A08);
	EXPORT_DL(bits_seg7_dl_07008B70);
	EXPORT_DL(bits_seg7_dl_07008C68);
	EXPORT_DL(bits_seg7_dl_07008D18);
	EXPORT_DL(bits_seg7_dl_07008F58);
	EXPORT_DL(bits_seg7_dl_07008FE8);
	EXPORT_DL(bits_seg7_dl_070094F8);
	EXPORT_DL(bits_seg7_dl_07009690);
	EXPORT_DL(bits_seg7_dl_0700AD60);
	EXPORT_DL(bits_seg7_dl_0700AE98);
	EXPORT_DL(bits_seg7_dl_0700B0E0);
	EXPORT_DL(bits_seg7_dl_0700B420);
	EXPORT_DL(bits_seg7_dl_0700B4A0);
	EXPORT_DL(bits_seg7_dl_0700B748);
	EXPORT_DL(bits_seg7_dl_0700B790);
	EXPORT_DL(bits_seg7_dl_0700B820);
	EXPORT_DL(bits_seg7_dl_0700B9B0);
	EXPORT_DL(bits_seg7_dl_0700BA18);
	EXPORT_DL(bits_seg7_dl_0700C908);
	EXPORT_DL(bits_seg7_dl_0700C998);
	EXPORT_DL(bits_seg7_dl_0700CA70);
	EXPORT_DL(bits_seg7_dl_0700CAF0);
	EXPORT_DL(bits_seg7_dl_0700CDA0);
	EXPORT_DL(bits_seg7_dl_0700CDC0);
	EXPORT_DL(bits_seg7_dl_0700D140);
	EXPORT_DL(bits_seg7_dl_0700D228);
	EXPORT_DL(bits_seg7_dl_0700D278);
	EXPORT_DL(bits_seg7_dl_0700D4D0);
	EXPORT_DL(bits_seg7_dl_0700D5A0);
	EXPORT_DL(bits_seg7_dl_0700DB20);
	EXPORT_DL(bits_seg7_dl_0700DC08);
	EXPORT_DL(bits_seg7_dl_0700DCB8);
	EXPORT_DL(bits_seg7_dl_0700DD00);
	EXPORT_DL(bits_seg7_dl_0700F568);
	EXPORT_DL(bits_seg7_dl_0700F790);
	EXPORT_DL(bits_seg7_dl_0700FC70);
	EXPORT_DL(bits_seg7_dl_07011EF8);
	EXPORT_DL(bits_seg7_dl_07012098);
	EXPORT_DL(bits_seg7_dl_07012758);
	EXPORT_DL(bits_seg7_dl_07012898);
	EXPORT_DL(bits_seg7_dl_070128F0);
	EXPORT_DL(bits_seg7_dl_07012AA8);
	EXPORT_DL(bits_seg7_dl_07012B10);
	EXPORT_DL(bits_seg7_dl_07012CC0);
	EXPORT_DL(bits_seg7_dl_07012D40);
	EXPORT_DL(bits_seg7_dl_070133B8);
	EXPORT_DL(bits_seg7_dl_070135A0);
	EXPORT_DL(bits_seg7_dl_07013790);
	EXPORT_DL(bits_seg7_dl_07013820);
	EXPORT_DL(bits_seg7_dl_07013BA8);
	EXPORT_DL(bits_seg7_dl_07013C78);
	EXPORT_DL(bits_seg7_dl_07013E68);
	EXPORT_DL(bits_seg7_dl_07013EF8);
	EXPORT_DL(bits_seg7_dl_070140E8);
	EXPORT_DL(bits_seg7_dl_07014178);
	EXPORT_DL(bits_seg7_dl_070149A8);
	EXPORT_DL(bits_seg7_dl_07014C28);
	EXPORT_DL(bits_seg7_dl_07015218);
	EXPORT_DL(bits_seg7_dl_07015250);
	EXPORT_DL(bits_seg7_dl_070153C0);
	EXPORT_DL(bits_seg7_dl_070159B8);
	EXPORT_DL(bits_seg7_dl_070159F0);
	EXPORT_DL(bits_seg7_dl_07015B60);
	EXPORT_DL(bits_seg7_dl_07016158);
	EXPORT_DL(bits_seg7_dl_07016190);
	EXPORT_DL(bits_seg7_dl_07016300);
	EXPORT_DL(bits_seg7_dl_070168F8);
	EXPORT_DL(bits_seg7_dl_07016930);
	EXPORT_DL(bits_seg7_dl_07016AA0);
	EXPORT_DL(bits_seg7_dl_07016CD8);
	EXPORT_DL(bits_seg7_dl_07016D10);
	EXPORT_DL(bits_seg7_dl_07016DA0);
}
