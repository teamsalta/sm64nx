#ifndef COMMON0_H
#define COMMON0_H

#include "types.h"
#include "engine/hook_geo.h"

// amp
namespace sm64::geo { const GeoLayout* amp_geo(); }
extern const Gfx amp_seg8_dl_08002B68[];
extern const Gfx amp_seg8_dl_08002BA0[];
extern const Gfx amp_seg8_dl_08002C50[];
extern const Gfx amp_seg8_dl_08002C88[];
extern const Gfx amp_seg8_dl_08002D38[];
extern const Gfx amp_seg8_dl_08002D70[];
extern const Gfx amp_seg8_dl_08002E20[];
extern const Gfx amp_seg8_dl_08002E58[];
extern const Gfx amp_seg8_dl_08003910[];
extern const Gfx amp_seg8_dl_08003940[];
extern const Gfx amp_seg8_dl_08003970[];
extern const Gfx amp_seg8_dl_080039A0[];
extern const Gfx amp_seg8_dl_080039D0[];
extern const Gfx amp_seg8_dl_08003DA8[];
extern const Gfx amp_seg8_dl_08003DD8[];
extern const Gfx amp_seg8_dl_08003E00[];
extern const struct Animation *const anim_amp[];

// blue_coin_switch
namespace sm64::geo { const GeoLayout* blue_coin_switch_geo(); }
extern const Gfx blue_coin_switch_seg8_dl_08000D58[];
extern const Gfx blue_coin_switch_seg8_dl_08000DD0[];
extern const Gfx blue_coin_switch_seg8_dl_08000E08[];
extern const s16 blue_coin_switch_seg8_collision_08000E98[];

// bobomb
namespace sm64::geo { const GeoLayout* black_bobomb_geo(); }
namespace sm64::geo { const GeoLayout* bobomb_buddy_geo(); }
extern const Gfx bobomb_seg8_dl_08022AC0[];
extern const Gfx bobomb_seg8_dl_08022B08[];
extern const Gfx bobomb_seg8_dl_08022B58[];
extern const Gfx bobomb_seg8_dl_08022B88[];
extern const Gfx bobomb_seg8_dl_08022C38[];
extern const Gfx bobomb_seg8_dl_08022CA0[];
extern const Gfx bobomb_seg8_dl_08022D08[];
extern const Gfx bobomb_seg8_dl_08022D78[];
extern const Gfx bobomb_seg8_dl_08023270[];
extern const Gfx bobomb_seg8_dl_08023378[];
extern const Gfx bobomb_seg8_dl_08023480[];
extern const struct Animation *const bobomb_seg8_anims_0802396C[];

// bowling_ball
namespace sm64::geo { const GeoLayout* bowling_ball_geo(); }
namespace sm64::geo { const GeoLayout* bowling_ball_track_geo(); }

// breakable_box
namespace sm64::geo { const GeoLayout* breakable_box_geo(); }
namespace sm64::geo { const GeoLayout* breakable_box_small_geo(); }
extern const Gfx breakable_box_seg8_dl_08012C30[];
extern const Gfx breakable_box_seg8_dl_08012CD8[];
extern const Gfx breakable_box_seg8_dl_08012D20[];
extern const Gfx breakable_box_seg8_dl_08012D48[];
extern const s16 breakable_box_seg8_collision_08012D70[];

// cannon_barrel
namespace sm64::geo { const GeoLayout* cannon_barrel_geo(); }
extern const Gfx cannon_barrel_seg8_dl_08006408[];
extern const Gfx cannon_barrel_seg8_dl_080064C0[];
extern const Gfx cannon_barrel_seg8_dl_08006660[];

// cannon_base
namespace sm64::geo { const GeoLayout* cannon_base_geo(); }
extern const Gfx cannon_base_seg8_dl_08005658[];
extern const Gfx cannon_base_seg8_dl_080056D0[];
extern const Gfx cannon_base_seg8_dl_080057F8[];

// cannon_lid
extern const Gfx cannon_lid_seg8_dl_08004898[];
extern const Gfx cannon_lid_seg8_dl_080048E0[];
extern const s16 cannon_lid_seg8_collision_08004950[];

// checkerboard_platform
namespace sm64::geo { const GeoLayout* checkerboard_platform_geo(); }
extern const Gfx checkerboard_platform_seg8_dl_0800D5C0[];
extern const Gfx checkerboard_platform_seg8_dl_0800D618[];
extern const Gfx checkerboard_platform_seg8_dl_0800D680[];
extern const s16 checkerboard_platform_seg8_collision_0800D710[];

// chuckya
namespace sm64::geo { const GeoLayout* chuckya_geo(); }
extern const Gfx chuckya_seg8_dl_08009FF8[];
extern const Gfx chuckya_seg8_dl_0800A030[];
extern const Gfx chuckya_seg8_dl_0800A068[];
extern const Gfx chuckya_seg8_dl_0800A160[];
extern const Gfx chuckya_seg8_dl_0800A198[];
extern const Gfx chuckya_seg8_dl_0800A1D0[];
extern const Gfx chuckya_seg8_dl_0800A2C8[];
extern const Gfx chuckya_seg8_dl_0800A300[];
extern const Gfx chuckya_seg8_dl_0800A338[];
extern const Gfx chuckya_seg8_dl_0800A3F0[];
extern const Gfx chuckya_seg8_dl_0800A428[];
extern const Gfx chuckya_seg8_dl_0800A4D8[];
extern const Gfx chuckya_seg8_dl_0800A510[];
extern const Gfx chuckya_seg8_dl_0800A5C0[];
extern const Gfx chuckya_seg8_dl_0800A5F8[];
extern const Gfx chuckya_seg8_dl_0800A700[];
extern const Gfx chuckya_seg8_dl_0800A758[];
extern const Gfx chuckya_seg8_dl_0800A870[];
extern const Gfx chuckya_seg8_dl_0800A8D0[];
extern const Gfx chuckya_seg8_dl_0800A958[];
extern const Gfx chuckya_seg8_dl_0800A998[];
extern const Gfx chuckya_seg8_dl_0800AB70[];
extern const Gfx chuckya_seg8_dl_0800ABE8[];
extern const struct Animation *const chuckya_seg8_anims_0800C070[];

// exclamation_box
namespace sm64::geo { const GeoLayout* exclamation_box_geo(); }
extern const Gfx exclamation_box_seg8_dl_08018FA8[];
extern const Gfx exclamation_box_seg8_dl_08019008[];
extern const Gfx exclamation_box_seg8_dl_08019058[];
extern const Gfx exclamation_box_seg8_dl_08019220[];
extern const Gfx exclamation_box_seg8_dl_08019280[];
extern const Gfx exclamation_box_seg8_dl_080192D0[];
extern const Gfx exclamation_box_seg8_dl_08019318[];
extern const Gfx exclamation_box_seg8_dl_08019378[];
extern const Gfx exclamation_box_seg8_dl_080193D8[];
extern const Gfx exclamation_box_seg8_dl_08019438[];

// exclamation_box_outline
namespace sm64::geo { const GeoLayout* exclamation_box_outline_geo(); }
extern const Gfx exclamation_box_outline_seg8_dl_08024EB8[];
extern const Gfx exclamation_box_outline_seg8_dl_08024F30[];
extern const Gfx exclamation_box_outline_seg8_dl_08024F58[];
extern const Gfx exclamation_box_outline_seg8_dl_08024F88[];
extern const Gfx exclamation_box_outline_seg8_dl_08024FA8[];
extern const Gfx exclamation_box_outline_seg8_dl_08024FC8[];
extern const Gfx exclamation_box_outline_seg8_dl_08024FE8[];
extern const Gfx exclamation_box_outline_seg8_dl_08025968[];
extern const Gfx exclamation_box_outline_seg8_dl_080259F8[];
extern const Gfx exclamation_box_outline_seg8_dl_08025EC0[];
extern const Gfx exclamation_box_outline_seg8_dl_08025F08[];
extern const s16 exclamation_box_outline_seg8_collision_08025F78[];

// flyguy
namespace sm64::geo { const GeoLayout* flyguy_geo(); }
extern const Gfx flyguy_seg8_dl_08010840[];
extern const Gfx flyguy_seg8_dl_08010968[];
extern const Gfx flyguy_seg8_dl_08010A90[];
extern const Gfx flyguy_seg8_dl_08010B38[];
extern const Gfx flyguy_seg8_dl_08010B80[];
extern const Gfx flyguy_seg8_dl_080113A8[];
extern const Gfx flyguy_seg8_dl_08011420[];
extern const Gfx flyguy_seg8_dl_080116D0[];
extern const Gfx flyguy_seg8_dl_08011710[];
extern const struct Animation *const flyguy_seg8_anims_08011A64[];

// goomba
namespace sm64::geo { const GeoLayout* goomba_geo(); }
extern const Gfx goomba_seg8_dl_0801B2E8[];
extern const Gfx goomba_seg8_dl_0801B560[];
extern const Gfx goomba_seg8_dl_0801B5A0[];
extern const Gfx goomba_seg8_dl_0801B5C8[];
extern const Gfx goomba_seg8_dl_0801B5F0[];
extern const Gfx goomba_seg8_dl_0801B658[];
extern const Gfx goomba_seg8_dl_0801B690[];
extern const Gfx goomba_seg8_dl_0801CE20[];
extern const Gfx goomba_seg8_dl_0801CF78[];
extern const Gfx goomba_seg8_dl_0801D0D0[];
extern const Gfx goomba_seg8_dl_0801D360[];
extern const Gfx goomba_seg8_dl_0801D760[];
extern const struct Animation *const goomba_seg8_anims_0801DA4C[];

// heart
namespace sm64::geo { const GeoLayout* heart_geo(); }
extern const Gfx heart_seg8_dl_0800DFE0[];

// koopa_shell
namespace sm64::geo { const GeoLayout* koopa_shell_geo(); }
namespace sm64::geo { const GeoLayout* koopa_shell2_geo(); }
namespace sm64::geo { const GeoLayout* koopa_shell3_geo(); }
extern const Gfx koopa_shell_seg8_dl_08027108[];
extern const Gfx koopa_shell_seg8_dl_08027170[];
extern const Gfx koopa_shell_seg8_dl_08027258[];
extern const Gfx koopa_shell_seg8_dl_080273C8[];
extern const Gfx koopa_shell_seg8_dl_08027420[];
extern const Gfx koopa_shell_seg8_dl_080288E0[];
extern const Gfx koopa_shell_seg8_dl_08028978[];
extern const Gfx koopa_shell_seg8_dl_08028A20[];
extern const Gfx koopa_shell_seg8_dl_08028B78[];

// metal_box
namespace sm64::geo { const GeoLayout* metal_box_geo(); }
extern const Gfx metal_box_seg8_dl_08024B18[];
extern const Gfx metal_box_dl[];
extern const s16 metal_box_seg8_collision_08024C28[];

// purple_switch
namespace sm64::geo { const GeoLayout* purple_switch_geo(); }
extern const Gfx purple_switch_seg8_dl_0800C668[];
extern const Gfx purple_switch_seg8_dl_0800C6E0[];
extern const Gfx purple_switch_seg8_dl_0800C718[];
extern const s16 purple_switch_seg8_collision_0800C7A8[];

// test_platform
namespace sm64::geo { const GeoLayout* test_platform_geo(); }
extern const Gfx unknown_seg8_dl_08026260[];
extern const s16 unknown_seg8_collision_080262F8[];

#endif
