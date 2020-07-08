#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "common0.h"
//#include "game/debug.h"


#include "blue_coin_switch/model.inc.c"
#include "blue_coin_switch/collision.inc.c"
UNUSED static const u64 binid_0 = 0;

#include "amp/model.inc.c"
#include "amp/anims/data.inc.c"
#include "amp/anims/table.inc.c"
UNUSED static const u64 binid_1 = 1;

#include "cannon_lid/model.inc.c"
#include "cannon_lid/collision.inc.c"
UNUSED static const u64 binid_2 = 2;

#include "cannon_base/model.inc.c"
UNUSED static const u64 binid_3 = 3;

#include "cannon_barrel/model.inc.c"
UNUSED static const u64 binid_4 = 4;

#include "chuckya/model.inc.c"
#include "chuckya/anims/data.inc.c"
#include "chuckya/anims/table.inc.c"
UNUSED static const u64 binid_5 = 5;

#include "purple_switch/model.inc.c"
#include "purple_switch/collision.inc.c"
UNUSED static const u64 binid_6 = 6;

#include "checkerboard_platform/model.inc.c"
#include "checkerboard_platform/collision.inc.c"
UNUSED static const u64 binid_7 = 7;

#include "heart/model.inc.c"
UNUSED static const u64 binid_8 = 8;

#include "flyguy/model.inc.c"
#include "flyguy/anims/data.inc.c"
#include "flyguy/anims/table.inc.c"
UNUSED static const u64 binid_9 = 9;

#include "breakable_box/model.inc.c"
#include "breakable_box/collision.inc.c"
UNUSED static const u64 binid_10 = 10;

UNUSED static const u64 binid_11 = 11;

#include "exclamation_box/model.inc.c"
UNUSED static const u64 binid_12 = 12;

#include "goomba/model.inc.c"
#include "goomba/anims/data.inc.c"
#include "goomba/anims/table.inc.c"
UNUSED static const u64 binid_13 = 13;

#include "bobomb/model.inc.c"
#include "bobomb/anims/data.inc.c"
#include "bobomb/anims/table.inc.c"
UNUSED static const u64 binid_14 = 14;

#include "metal_box/model.inc.c"
#include "metal_box/collision.inc.c"
UNUSED static const u64 binid_15 = 15;

#include "exclamation_box_outline/model.inc.c"
#include "exclamation_box_outline/collision.inc.c"
UNUSED static const u64 binid_16 = 16;

#include "test_platform/model.inc.c"
#include "test_platform/collision.inc.c"
UNUSED static const u64 binid_17 = 17;

#include "koopa_shell/model.inc.c"
UNUSED static const u64 binid_18 = 18;

#include "game/debug.h"
namespace sm64
{
	EXPORT_DL(blue_coin_switch_seg8_dl_08000D58);
	EXPORT_DL(blue_coin_switch_seg8_dl_08000DD0);
	EXPORT_DL(blue_coin_switch_seg8_dl_08000E08);
	EXPORT_DL(amp_seg8_dl_08002B68);
	EXPORT_DL(amp_seg8_dl_08002BA0);
	EXPORT_DL(amp_seg8_dl_08002C50);
	EXPORT_DL(amp_seg8_dl_08002C88);
	EXPORT_DL(amp_seg8_dl_08002D38);
	EXPORT_DL(amp_seg8_dl_08002D70);
	EXPORT_DL(amp_seg8_dl_08002E20);
	EXPORT_DL(amp_seg8_dl_08002E58);
	EXPORT_DL(amp_seg8_dl_08003910);
	EXPORT_DL(amp_seg8_dl_08003940);
	EXPORT_DL(amp_seg8_dl_08003970);
	EXPORT_DL(amp_seg8_dl_080039A0);
	EXPORT_DL(amp_seg8_dl_080039D0);
	EXPORT_DL(amp_seg8_dl_08003DA8);
	EXPORT_DL(amp_seg8_dl_08003DD8);
	EXPORT_DL(amp_seg8_dl_08003E00);
	EXPORT_DL(cannon_lid_seg8_dl_08004898);
	EXPORT_DL(cannon_lid_seg8_dl_080048E0);
	EXPORT_DL(cannon_base_seg8_dl_08005658);
	EXPORT_DL(cannon_base_seg8_dl_080056D0);
	EXPORT_DL(cannon_base_seg8_dl_080057F8);
	EXPORT_DL(cannon_barrel_seg8_dl_08006408);
	EXPORT_DL(cannon_barrel_seg8_dl_080064C0);
	EXPORT_DL(cannon_barrel_seg8_dl_08006660);
	EXPORT_DL(chuckya_seg8_dl_08009FF8);
	EXPORT_DL(chuckya_seg8_dl_0800A030);
	EXPORT_DL(chuckya_seg8_dl_0800A068);
	EXPORT_DL(chuckya_seg8_dl_0800A160);
	EXPORT_DL(chuckya_seg8_dl_0800A198);
	EXPORT_DL(chuckya_seg8_dl_0800A1D0);
	EXPORT_DL(chuckya_seg8_dl_0800A2C8);
	EXPORT_DL(chuckya_seg8_dl_0800A300);
	EXPORT_DL(chuckya_seg8_dl_0800A338);
	EXPORT_DL(chuckya_seg8_dl_0800A3F0);
	EXPORT_DL(chuckya_seg8_dl_0800A428);
	EXPORT_DL(chuckya_seg8_dl_0800A4D8);
	EXPORT_DL(chuckya_seg8_dl_0800A510);
	EXPORT_DL(chuckya_seg8_dl_0800A5C0);
	EXPORT_DL(chuckya_seg8_dl_0800A5F8);
	EXPORT_DL(chuckya_seg8_dl_0800A700);
	EXPORT_DL(chuckya_seg8_dl_0800A758);
	EXPORT_DL(chuckya_seg8_dl_0800A870);
	EXPORT_DL(chuckya_seg8_dl_0800A8D0);
	EXPORT_DL(chuckya_seg8_dl_0800A958);
	EXPORT_DL(chuckya_seg8_dl_0800A998);
	EXPORT_DL(chuckya_seg8_dl_0800AB70);
	EXPORT_DL(chuckya_seg8_dl_0800ABE8);
	EXPORT_DL(purple_switch_seg8_dl_0800C668);
	EXPORT_DL(purple_switch_seg8_dl_0800C6E0);
	EXPORT_DL(purple_switch_seg8_dl_0800C718);
	EXPORT_DL(checkerboard_platform_seg8_dl_0800D5C0);
	EXPORT_DL(checkerboard_platform_seg8_dl_0800D618);
	EXPORT_DL(checkerboard_platform_seg8_dl_0800D680);
	EXPORT_DL(heart_seg8_dl_0800DFE0);
	EXPORT_DL(flyguy_seg8_dl_08010840);
	EXPORT_DL(flyguy_seg8_dl_08010968);
	EXPORT_DL(flyguy_seg8_dl_08010A90);
	EXPORT_DL(flyguy_seg8_dl_08010B38);
	EXPORT_DL(flyguy_seg8_dl_08010B80);
	EXPORT_DL(flyguy_seg8_dl_080113A8);
	EXPORT_DL(flyguy_seg8_dl_08011420);
	EXPORT_DL(flyguy_seg8_dl_080116D0);
	EXPORT_DL(flyguy_seg8_dl_08011710);
	EXPORT_DL(breakable_box_seg8_dl_08012C30);
	EXPORT_DL(breakable_box_seg8_dl_08012CD8);
	EXPORT_DL(breakable_box_seg8_dl_08012D20);
	EXPORT_DL(breakable_box_seg8_dl_08012D48);
	EXPORT_DL(exclamation_box_seg8_dl_08018FA8);
	EXPORT_DL(exclamation_box_seg8_dl_08019008);
	EXPORT_DL(exclamation_box_seg8_dl_08019058);
	EXPORT_DL(exclamation_box_seg8_dl_08019220);
	EXPORT_DL(exclamation_box_seg8_dl_08019280);
	EXPORT_DL(exclamation_box_seg8_dl_080192D0);
	EXPORT_DL(exclamation_box_seg8_dl_08019318);
	EXPORT_DL(exclamation_box_seg8_dl_08019378);
	EXPORT_DL(exclamation_box_seg8_dl_080193D8);
	EXPORT_DL(exclamation_box_seg8_dl_08019438);
	EXPORT_DL(goomba_seg8_dl_0801B2E8);
	EXPORT_DL(goomba_seg8_dl_0801B560);
	EXPORT_DL(goomba_seg8_dl_0801B5A0);
	EXPORT_DL(goomba_seg8_dl_0801B5C8);
	EXPORT_DL(goomba_seg8_dl_0801B5F0);
	EXPORT_DL(goomba_seg8_dl_0801B658);
	EXPORT_DL(goomba_seg8_dl_0801B690);
	EXPORT_DL(goomba_seg8_dl_0801CE20);
	EXPORT_DL(goomba_seg8_dl_0801CF78);
	EXPORT_DL(goomba_seg8_dl_0801D0D0);
	EXPORT_DL(goomba_seg8_dl_0801D360);
	EXPORT_DL(goomba_seg8_dl_0801D760);
	EXPORT_DL(bobomb_seg8_dl_08022AC0);
	EXPORT_DL(bobomb_seg8_dl_08022B08);
	EXPORT_DL(bobomb_seg8_dl_08022B58);
	EXPORT_DL(bobomb_seg8_dl_08022B88);
	EXPORT_DL(bobomb_seg8_dl_08022C38);
	EXPORT_DL(bobomb_seg8_dl_08022CA0);
	EXPORT_DL(bobomb_seg8_dl_08022D08);
	EXPORT_DL(bobomb_seg8_dl_08022D78);
	EXPORT_DL(bobomb_seg8_dl_08023270);
	EXPORT_DL(bobomb_seg8_dl_08023378);
	EXPORT_DL(bobomb_seg8_dl_08023480);
	EXPORT_DL(metal_box_seg8_dl_08024B18);
	EXPORT_DL(metal_box_dl);
	EXPORT_DL(exclamation_box_outline_seg8_dl_08024EB8);
	EXPORT_DL(exclamation_box_outline_seg8_dl_08024F30);
	EXPORT_DL(exclamation_box_outline_seg8_dl_08024F58);
	EXPORT_DL(exclamation_box_outline_seg8_dl_08024F88);
	EXPORT_DL(exclamation_box_outline_seg8_dl_08024FA8);
	EXPORT_DL(exclamation_box_outline_seg8_dl_08024FC8);
	EXPORT_DL(exclamation_box_outline_seg8_dl_08024FE8);
	EXPORT_DL(exclamation_box_outline_seg8_dl_08025968);
	EXPORT_DL(exclamation_box_outline_seg8_dl_080259F8);
	EXPORT_DL(exclamation_box_outline_seg8_dl_08025EC0);
	EXPORT_DL(exclamation_box_outline_seg8_dl_08025F08);
	EXPORT_DL(unknown_seg8_dl_08026260);
	EXPORT_DL(koopa_shell_seg8_dl_08027108);
	EXPORT_DL(koopa_shell_seg8_dl_08027170);
	EXPORT_DL(koopa_shell_seg8_dl_08027258);
	EXPORT_DL(koopa_shell_seg8_dl_080273C8);
	EXPORT_DL(koopa_shell_seg8_dl_08027420);
	EXPORT_DL(koopa_shell_seg8_dl_080288E0);
	EXPORT_DL(koopa_shell_seg8_dl_08028978);
	EXPORT_DL(koopa_shell_seg8_dl_08028A20);
	EXPORT_DL(koopa_shell_seg8_dl_08028B78);
}
