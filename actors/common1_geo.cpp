#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"

#include "mist/geo.inc.c"
#include "explosion/geo.inc.c"
#include "butterfly/geo.inc.c"
#include "coin/geo.inc.c"
#include "warp_pipe/geo.inc.c"
#include "door/geo.inc.c"
#include "bowser_key/geo.inc.c"
#include "flame/geo.inc.c"
#include "blue_fish/geo.inc.c"
#include "leaves/geo.inc.c"
#include "mario_cap/geo.inc.c"
#include "number/geo.inc.c"
#include "mushroom_1up/geo.inc.c"
#include "star/geo.inc.c"
#include "dirt/geo.inc.c"
#include "transparent_star/geo.inc.c"
#include "white_particle/geo.inc.c"
#include "wooden_signpost/geo.inc.c"
#include "tree/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(mist_geo);
	EXPORT_GEO(white_puff_geo);
	EXPORT_GEO(explosion_geo);
	EXPORT_GEO(butterfly_geo);
	EXPORT_GEO(yellow_coin_geo);
	EXPORT_GEO(yellow_coin_no_shadow_geo);
	EXPORT_GEO(blue_coin_geo);
	EXPORT_GEO(blue_coin_no_shadow_geo);
	EXPORT_GEO(red_coin_geo);
	EXPORT_GEO(red_coin_no_shadow_geo);
	EXPORT_GEO(warp_pipe_geo);
	EXPORT_GEO(castle_door_geo);
	EXPORT_GEO(cabin_door_geo);
	EXPORT_GEO(wooden_door_geo);
	EXPORT_GEO(wooden_door2_geo);
	EXPORT_GEO(metal_door_geo);
	EXPORT_GEO(hazy_maze_door_geo);
	EXPORT_GEO(haunted_door_geo);
	EXPORT_GEO(castle_door_0_star_geo);
	EXPORT_GEO(castle_door_1_star_geo);
	EXPORT_GEO(castle_door_3_stars_geo);
	EXPORT_GEO(key_door_geo);
	EXPORT_GEO(bowser_key_geo);
	EXPORT_GEO(bowser_key_cutscene_geo);
	EXPORT_GEO(red_flame_shadow_geo);
	EXPORT_GEO(red_flame_geo);
	EXPORT_GEO(blue_flame_geo);
	EXPORT_GEO(fish_shadow_geo);
	EXPORT_GEO(fish_geo);
	EXPORT_GEO(leaves_geo);
	EXPORT_GEO(number_geo);
	EXPORT_GEO(mushroom_1up_geo);
	EXPORT_GEO(star_geo);
	EXPORT_GEO(dirt_animation_geo);
	EXPORT_GEO(cartoon_star_geo);
	EXPORT_GEO(transparent_star_geo);
	EXPORT_GEO(white_particle_geo);
	EXPORT_GEO(wooden_signpost_geo);
	EXPORT_GEO(bubbly_tree_geo);
	EXPORT_GEO(spiky_tree_geo);
	EXPORT_GEO(snow_tree_geo);
	EXPORT_GEO(spiky_tree1_geo);
	EXPORT_GEO(palm_tree_geo);
}

