#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "common0.h"
#include "engine/asset.h"



#include "mist/model.inc.c"
UNUSED static const u64 binid_0 = 0;

#include "explosion/model.inc.c"
UNUSED static const u64 binid_1 = 1;

#include "butterfly/model.inc.c"
#include "butterfly/anims/data.inc.c"
#include "butterfly/anims/table.inc.c"
UNUSED static const u64 binid_2 = 2;

#include "coin/model.inc.c"
UNUSED static const u64 binid_3 = 3;

#include "warp_pipe/model.inc.c"
#include "warp_pipe/collision.inc.c"
UNUSED static const u64 binid_4 = 4;

#include "door/model.inc.c"
#include "door/anims/data.inc.c"
#include "door/anims/table.inc.c"
UNUSED static const u64 binid_5 = 5;

#include "bowser_key/model.inc.c"
#include "bowser_key/anims/data.inc.c"
#include "bowser_key/anims/table.inc.c"
UNUSED static const u64 binid_6 = 6;

#include "flame/model.inc.c"
UNUSED static const u64 binid_7 = 7;

#include "blue_fish/model.inc.c"
#include "blue_fish/anims/data.inc.c"
#include "blue_fish/anims/table.inc.c"
UNUSED static const u64 binid_8 = 8;

#include "pebble/model.inc.c"
UNUSED static const u64 binid_9 = 9;

#include "leaves/model.inc.c"
UNUSED static const u64 binid_10 = 10;

// this is still weird...
#include "warp_collision/collision.inc.c"
UNUSED static const u64 binid_11 = 11;

#include "mario_cap/model.inc.c"
UNUSED static const u64 binid_12 = 12;

#include "power_meter/model.inc.c"
UNUSED static const u64 binid_13 = 13;

UNUSED static const u64 binid_14 = 14;

#include "mushroom_1up/model.inc.c"
UNUSED static const u64 binid_15 = 15;

#include "star/model.inc.c"
UNUSED static const u64 binid_16 = 16;

#include "sand/model.inc.c"
UNUSED static const u64 binid_17 = 17;

#include "dirt/model.inc.c"
UNUSED static const u64 binid_18 = 18;

#include "transparent_star/model.inc.c"
UNUSED static const u64 binid_19 = 19;

#include "white_particle/model.inc.c"
UNUSED static const u64 binid_20 = 20;

#include "wooden_signpost/model.inc.c"
#include "wooden_signpost/collision.inc.c"
UNUSED static const u64 binid_21 = 21;

#include "tree/model.inc.c"
UNUSED static const u64 binid_22 = 22;

#include "game/debug.h"
namespace sm64
{
	EXPORT_GEO(marios_winged_metal_cap_geo);
	EXPORT_GEO(marios_metal_cap_geo);
	EXPORT_GEO(marios_wing_cap_geo);
	EXPORT_GEO(marios_cap_geo);

	EXPORT_DL(mist_seg3_dl_03000880);
	EXPORT_DL(mist_seg3_dl_03000920);
	EXPORT_DL(explosion_seg3_dl_03004208);
	EXPORT_DL(explosion_seg3_dl_03004298);
	EXPORT_DL(explosion_seg3_dl_030042B0);
	EXPORT_DL(explosion_seg3_dl_030042C8);
	EXPORT_DL(explosion_seg3_dl_030042E0);
	EXPORT_DL(explosion_seg3_dl_030042F8);
	EXPORT_DL(explosion_seg3_dl_03004310);
	EXPORT_DL(explosion_seg3_dl_03004328);
	EXPORT_DL(butterfly_seg3_dl_03005408);
	EXPORT_DL(butterfly_seg3_dl_030054A0);
	EXPORT_DL(coin_seg3_dl_03007780);
	EXPORT_DL(coin_seg3_dl_030077D0);
	EXPORT_DL(coin_seg3_dl_03007800);
	EXPORT_DL(coin_seg3_dl_03007828);
	EXPORT_DL(coin_seg3_dl_03007850);
	EXPORT_DL(coin_seg3_dl_03007878);
	EXPORT_DL(coin_seg3_dl_030078A0);
	EXPORT_DL(coin_seg3_dl_030078C8);
	EXPORT_DL(coin_seg3_dl_030078F0);
	EXPORT_DL(coin_seg3_dl_03007918);
	EXPORT_DL(coin_seg3_dl_03007940);
	EXPORT_DL(coin_seg3_dl_03007968);
	EXPORT_DL(coin_seg3_dl_03007990);
	EXPORT_DL(coin_seg3_dl_030079B8);
	EXPORT_DL(warp_pipe_seg3_dl_03008E40);
	EXPORT_DL(warp_pipe_seg3_dl_03008F98);
	EXPORT_DL(warp_pipe_seg3_dl_03009968);
	EXPORT_DL(warp_pipe_seg3_dl_03009A20);
	EXPORT_DL(warp_pipe_seg3_dl_03009A50);
	EXPORT_DL(door_seg3_dl_03013C10);
	EXPORT_DL(door_seg3_dl_03013CC8);
	EXPORT_DL(door_seg3_dl_03013D78);
	EXPORT_DL(door_seg3_dl_03013E28);
	EXPORT_DL(door_seg3_dl_03013EA8);
	EXPORT_DL(door_seg3_dl_03014020);
	EXPORT_DL(door_seg3_dl_03014100);
	EXPORT_DL(door_seg3_dl_03014128);
	EXPORT_DL(door_seg3_dl_030141C0);
	EXPORT_DL(door_seg3_dl_03014218);
	EXPORT_DL(door_seg3_dl_03014250);
	EXPORT_DL(door_seg3_dl_03014280);
	EXPORT_DL(door_seg3_dl_030142B0);
	EXPORT_DL(door_seg3_dl_030142E0);
	EXPORT_DL(door_seg3_dl_03014310);
	EXPORT_DL(door_seg3_dl_03014340);
	EXPORT_DL(door_seg3_dl_03014470);
	EXPORT_DL(door_seg3_dl_030144E0);
	EXPORT_DL(door_seg3_dl_03014528);
	EXPORT_DL(door_seg3_dl_03014540);
	EXPORT_DL(door_seg3_dl_03014888);
	EXPORT_DL(door_seg3_dl_030149C0);
	EXPORT_DL(door_seg3_dl_03014A20);
	EXPORT_DL(door_seg3_dl_03014A50);
	EXPORT_DL(door_seg3_dl_03014A80);
	EXPORT_DL(door_seg3_dl_03014B30);
	EXPORT_DL(door_seg3_dl_03014BE0);
	EXPORT_DL(door_seg3_dl_03014C90);
	EXPORT_DL(door_seg3_dl_03014D40);
	EXPORT_DL(door_seg3_dl_03014EF0);
	EXPORT_DL(door_seg3_dl_03014F30);
	EXPORT_DL(door_seg3_dl_03014F68);
	EXPORT_DL(door_seg3_dl_03014F98);
	EXPORT_DL(door_seg3_dl_03015008);
	EXPORT_DL(door_seg3_dl_03015078);
	EXPORT_DL(door_seg3_dl_030150E8);
	EXPORT_DL(door_seg3_dl_03015158);
	EXPORT_DL(bowser_key_dl);
	EXPORT_DL(flame_seg3_dl_0301B320);
	EXPORT_DL(flame_seg3_dl_0301B3B0);
	EXPORT_DL(flame_seg3_dl_0301B3C8);
	EXPORT_DL(flame_seg3_dl_0301B3E0);
	EXPORT_DL(flame_seg3_dl_0301B3F8);
	EXPORT_DL(flame_seg3_dl_0301B410);
	EXPORT_DL(flame_seg3_dl_0301B428);
	EXPORT_DL(flame_seg3_dl_0301B440);
	EXPORT_DL(flame_seg3_dl_0301B458);
	EXPORT_DL(flame_seg3_dl_0301B470);
	EXPORT_DL(flame_seg3_dl_0301B500);
	EXPORT_DL(flame_seg3_dl_0301B518);
	EXPORT_DL(flame_seg3_dl_0301B530);
	EXPORT_DL(flame_seg3_dl_0301B548);
	EXPORT_DL(flame_seg3_dl_0301B560);
	EXPORT_DL(flame_seg3_dl_0301B578);
	EXPORT_DL(flame_seg3_dl_0301B590);
	EXPORT_DL(flame_seg3_dl_0301B5A8);
	EXPORT_DL(blue_fish_seg3_dl_0301BEC0);
	EXPORT_DL(blue_fish_seg3_dl_0301BFB8);
	EXPORT_DL(blue_fish_seg3_dl_0301C0A8);
	EXPORT_DL(blue_fish_seg3_dl_0301C150);
	EXPORT_DL(pebble_seg3_dl_0301CB00);
	EXPORT_DL(leaves_seg3_dl_0301CDE0);
	EXPORT_DL(dl_power_meter_base);
	EXPORT_DL(dl_power_meter_health_segments_begin);
	EXPORT_DL(dl_power_meter_health_segments_end);
	EXPORT_DL(mushroom_1up_seg3_dl_0302A628);
	EXPORT_DL(mushroom_1up_seg3_dl_0302A660);
	EXPORT_DL(star_seg3_dl_0302B7B0);
	EXPORT_DL(star_seg3_dl_0302B870);
	EXPORT_DL(star_seg3_dl_0302B9C0);
	EXPORT_DL(star_seg3_dl_0302BA18);
	EXPORT_DL(sand_seg3_dl_0302BCD0);
	EXPORT_DL(dirt_seg3_dl_0302BFF8);
	EXPORT_DL(dirt_seg3_dl_0302C028);
	EXPORT_DL(dirt_seg3_dl_0302C238);
	EXPORT_DL(dirt_seg3_dl_0302C298);
	EXPORT_DL(dirt_seg3_dl_0302C2B8);
	EXPORT_DL(dirt_seg3_dl_0302C2D8);
	EXPORT_DL(dirt_seg3_dl_0302C2F8);
	EXPORT_DL(dirt_seg3_dl_0302C318);
	EXPORT_DL(dirt_seg3_dl_0302C378);
	EXPORT_DL(dirt_seg3_dl_0302C3B0);
	EXPORT_DL(dirt_seg3_dl_0302C3E8);
	EXPORT_DL(dirt_seg3_dl_0302C420);
	EXPORT_DL(dirt_seg3_dl_0302C458);
	EXPORT_DL(transparent_star_seg3_dl_0302C560);
	EXPORT_DL(transparent_star_seg3_dl_0302C620);
	EXPORT_DL(white_particle_dl);
	EXPORT_DL(wooden_signpost_seg3_dl_0302D9C8);
	EXPORT_DL(wooden_signpost_seg3_dl_0302DA48);
	EXPORT_DL(wooden_signpost_seg3_dl_0302DC40);
	EXPORT_DL(wooden_signpost_seg3_dl_0302DCD0);
	EXPORT_DL(wooden_signpost_seg3_dl_0302DD08);
	EXPORT_DL(tree_seg3_dl_0302FE88);
	EXPORT_DL(tree_seg3_dl_0302FEB8);
	EXPORT_DL(tree_seg3_dl_0302FEE8);
	EXPORT_DL(tree_seg3_dl_03030FA0);
	EXPORT_DL(tree_seg3_dl_03032088);
	EXPORT_DL(tree_seg3_dl_03032170);
	EXPORT_DL(tree_seg3_dl_03033258);
}
