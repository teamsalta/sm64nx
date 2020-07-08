#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "group0.h"

#include "make_const_nonconst.h"

// Note: This bin does not use bin IDs, unlike the other segmented bins.
#include "mario/model.inc.c"

#include "bubble/model.inc.c"

#include "walk_smoke/model.inc.c"

#include "burn_smoke/model.inc.c"

#include "stomp_smoke/model.inc.c"

#include "water_waves/model.inc.c"

#include "sparkle/model.inc.c"

#include "water_splash/model.inc.c"

#include "white_particle_small/model.inc.c"

#include "sparkle_animation/model.inc.c"

#include "game/debug.h"

#ifdef DEBUG
namespace sm64::models::low
{
	EXPORT_DL(mario_butt);
	EXPORT_DL(mario_butt_dl);
	EXPORT_DL(mario_cap_off_eyes_closed);
	EXPORT_DL(mario_cap_off_eyes_dead);
	EXPORT_DL(mario_cap_off_eyes_down);
	EXPORT_DL(mario_cap_off_eyes_front);
	EXPORT_DL(mario_cap_off_eyes_half_closed);
	EXPORT_DL(mario_cap_off_eyes_left);
	EXPORT_DL(mario_cap_off_eyes_right);
	EXPORT_DL(mario_cap_off_eyes_up);
	EXPORT_DL(mario_cap_on_eyes_closed);
	EXPORT_DL(mario_cap_on_eyes_dead);
	EXPORT_DL(mario_cap_on_eyes_down);
	EXPORT_DL(mario_cap_on_eyes_front);
	EXPORT_DL(mario_cap_on_eyes_half_closed);
	EXPORT_DL(mario_cap_on_eyes_left);
	EXPORT_DL(mario_cap_on_eyes_right);
	EXPORT_DL(mario_cap_on_eyes_up);
	EXPORT_DL(mario_cap_unused_base_bottom_dl);
	EXPORT_DL(mario_cap_unused_base_dl);
	EXPORT_DL(mario_cap_unused_base_top_dl);
	EXPORT_DL(mario_cap_unused_dl);
	EXPORT_DL(mario_cap_unused_m_logo_dl);
	EXPORT_DL(mario_cap_wings);
	EXPORT_DL(mario_cap_wings_transparent);
	EXPORT_DL(mario_cap_wings_unused);
	EXPORT_DL(mario_cap_wings_unused_end_dl);
	EXPORT_DL(mario_cap_wings_unused_intial_dl);
	EXPORT_DL(mario_eyes_cap_off_dl);
	EXPORT_DL(mario_eyes_cap_on_dl);
	EXPORT_DL(mario_face_back_hair_cap_on_dl);
	EXPORT_DL(mario_face_cap_dl);
	EXPORT_DL(mario_face_cap_off_dl);
	EXPORT_DL(mario_face_cap_on_dl);
	EXPORT_DL(mario_face_hair_cap_off_dl);
	EXPORT_DL(mario_face_part_cap_off_dl);
	EXPORT_DL(mario_face_part_cap_on_dl);
	EXPORT_DL(mario_hair_sideburn_cap_off_dl);
	EXPORT_DL(mario_hair_sideburn_cap_on_dl);
	EXPORT_DL(mario_left_arm);
	EXPORT_DL(mario_left_arm_shared_dl);
	EXPORT_DL(mario_left_foot);
	EXPORT_DL(mario_left_foot_shared_dl);
	EXPORT_DL(mario_left_forearm_shared_dl);
	EXPORT_DL(mario_left_hand_closed);
	EXPORT_DL(mario_left_hand_closed_shared_dl);
	EXPORT_DL(mario_left_hand_open);
	EXPORT_DL(mario_left_hand_open_shared_dl);
	EXPORT_DL(mario_left_leg_shared_dl);
	EXPORT_DL(mario_left_thigh);
	EXPORT_DL(mario_left_thigh_dl);
	EXPORT_DL(mario_low_poly_butt);
	EXPORT_DL(mario_low_poly_butt_dl);
	EXPORT_DL(mario_low_poly_cap_off_eyes_closed);
	EXPORT_DL(mario_low_poly_cap_off_eyes_dead);
	EXPORT_DL(mario_low_poly_cap_off_eyes_down);
	EXPORT_DL(mario_low_poly_cap_off_eyes_front);
	EXPORT_DL(mario_low_poly_cap_off_eyes_half_closed);
	EXPORT_DL(mario_low_poly_cap_off_eyes_left);
	EXPORT_DL(mario_low_poly_cap_off_eyes_right);
	EXPORT_DL(mario_low_poly_cap_off_eyes_up);
	EXPORT_DL(mario_low_poly_cap_on_eyes_closed);
	EXPORT_DL(mario_low_poly_cap_on_eyes_dead);
	EXPORT_DL(mario_low_poly_cap_on_eyes_down);
	EXPORT_DL(mario_low_poly_cap_on_eyes_front);
	EXPORT_DL(mario_low_poly_cap_on_eyes_half_closed);
	EXPORT_DL(mario_low_poly_cap_on_eyes_left);
	EXPORT_DL(mario_low_poly_cap_on_eyes_right);
	EXPORT_DL(mario_low_poly_cap_on_eyes_up);
	EXPORT_DL(mario_low_poly_eyes_cap_on_dl);
	EXPORT_DL(mario_low_poly_face_back_hair_cap_on_dl);
	EXPORT_DL(mario_low_poly_face_cap_dl);
	EXPORT_DL(mario_low_poly_face_cap_off_dl);
	EXPORT_DL(mario_low_poly_face_cap_on_dl);
	EXPORT_DL(mario_low_poly_face_hair_cap_off_dl);
	EXPORT_DL(mario_low_poly_face_part_cap_off_dl);
	EXPORT_DL(mario_low_poly_face_part_cap_on_dl);
	EXPORT_DL(mario_low_poly_left_arm);
	EXPORT_DL(mario_low_poly_left_arm_shared_dl);
	EXPORT_DL(mario_low_poly_left_foot);
	EXPORT_DL(mario_low_poly_left_foot_shared_dl);
	EXPORT_DL(mario_low_poly_left_forearm_shared_dl);
	EXPORT_DL(mario_low_poly_left_hand_closed);
	EXPORT_DL(mario_low_poly_left_hand_closed_shared_dl);
	EXPORT_DL(mario_low_poly_left_leg_shared_dl);
	EXPORT_DL(mario_low_poly_left_thigh);
	EXPORT_DL(mario_low_poly_left_thigh_dl);
	EXPORT_DL(mario_low_poly_mario_eyes_cap_off_dl);
	EXPORT_DL(mario_low_poly_mario_m_logo_dl);
	EXPORT_DL(mario_low_poly_mustache_cap_off_dl);
	EXPORT_DL(mario_low_poly_mustache_cap_on_dl);
	EXPORT_DL(mario_low_poly_pants_overalls_shared_dl);
	EXPORT_DL(mario_low_poly_right_arm);
	EXPORT_DL(mario_low_poly_right_arm_shared_dl);
	EXPORT_DL(mario_low_poly_right_foot);
	EXPORT_DL(mario_low_poly_right_foot_dl);
	EXPORT_DL(mario_low_poly_right_forearm_shared_dl);
	EXPORT_DL(mario_low_poly_right_hand_closed);
	EXPORT_DL(mario_low_poly_right_hand_closed_dl);
	EXPORT_DL(mario_low_poly_right_leg_shared_dl);
	EXPORT_DL(mario_low_poly_right_thigh);
	EXPORT_DL(mario_low_poly_right_thigh_shared_dl);
	EXPORT_DL(mario_low_poly_torso);
	EXPORT_DL(mario_low_poly_torso_dl);
	EXPORT_DL(mario_low_poly_tshirt_shared_dl);
	EXPORT_DL(mario_low_poly_yellow_button_dl);
	EXPORT_DL(mario_m_logo_dl);
	EXPORT_DL(mario_medium_poly_butt);
	EXPORT_DL(mario_medium_poly_butt_dl);
	EXPORT_DL(mario_medium_poly_left_arm);
	EXPORT_DL(mario_medium_poly_left_arm_shared_dl);
	EXPORT_DL(mario_medium_poly_left_foot);
	EXPORT_DL(mario_medium_poly_left_foot_shared_dl);
	EXPORT_DL(mario_medium_poly_left_forearm_shared_dl);
	EXPORT_DL(mario_medium_poly_left_hand_closed);
	EXPORT_DL(mario_medium_poly_left_hand_closed_shared_dl);
	EXPORT_DL(mario_medium_poly_left_leg_shared_dl);
	EXPORT_DL(mario_medium_poly_left_thigh);
	EXPORT_DL(mario_medium_poly_left_thigh_dl);
	EXPORT_DL(mario_medium_poly_pants_overalls_shared_dl);
	EXPORT_DL(mario_medium_poly_right_arm);
	EXPORT_DL(mario_medium_poly_right_arm_shared_dl);
	EXPORT_DL(mario_medium_poly_right_foot);
	EXPORT_DL(mario_medium_poly_right_foot_dl);
	EXPORT_DL(mario_medium_poly_right_forearm_shared_dl);
	EXPORT_DL(mario_medium_poly_right_hand_closed);
	EXPORT_DL(mario_medium_poly_right_hand_closed_dl);
	EXPORT_DL(mario_medium_poly_right_leg_shared_dl);
	EXPORT_DL(mario_medium_poly_right_thigh);
	EXPORT_DL(mario_medium_poly_right_thigh_shared_dl);
	EXPORT_DL(mario_medium_poly_torso);
	EXPORT_DL(mario_medium_poly_torso_dl);
	EXPORT_DL(mario_medium_poly_tshirt_shared_dl);
	EXPORT_DL(mario_medium_poly_yellow_button_dl);
	EXPORT_DL(mario_metal_butt);
	EXPORT_DL(mario_metal_cap_off_shared_dl);
	EXPORT_DL(mario_metal_cap_on_shared_dl);
	EXPORT_DL(mario_metal_cap_unused_dl);
	EXPORT_DL(mario_metal_cap_wings);
	EXPORT_DL(mario_metal_cap_wings_transparent);
	EXPORT_DL(mario_metal_cap_wings_unused);
	EXPORT_DL(mario_metal_left_thigh);
	EXPORT_DL(mario_metal_low_poly_butt);
	EXPORT_DL(mario_metal_low_poly_cap_off);
	EXPORT_DL(mario_metal_low_poly_cap_on);
	EXPORT_DL(mario_metal_low_poly_left_thigh);
	EXPORT_DL(mario_metal_low_poly_right_foot);
	EXPORT_DL(mario_metal_low_poly_right_hand_closed);
	EXPORT_DL(mario_metal_low_poly_torso);
	EXPORT_DL(mario_metal_medium_poly_butt);
	EXPORT_DL(mario_metal_medium_poly_left_thigh);
	EXPORT_DL(mario_metal_medium_poly_right_foot);
	EXPORT_DL(mario_metal_medium_poly_right_hand_closed);
	EXPORT_DL(mario_metal_medium_poly_torso);
	EXPORT_DL(mario_metal_right_foot);
	EXPORT_DL(mario_metal_right_hand_cap_shared_dl);
	EXPORT_DL(mario_metal_right_hand_cap_shared_dl_wings);
	EXPORT_DL(mario_metal_right_hand_closed);
	EXPORT_DL(mario_metal_right_hand_open);
	EXPORT_DL(mario_metal_torso_shared_dl);
	EXPORT_DL(mario_mustache_cap_off_dl);
	EXPORT_DL(mario_mustache_cap_on_dl);
	EXPORT_DL(mario_pants_overalls_shared_dl);
	EXPORT_DL(mario_right_arm);
	EXPORT_DL(mario_right_arm_shared_dl);
	EXPORT_DL(mario_right_foot);
	EXPORT_DL(mario_right_foot_dl);
	EXPORT_DL(mario_right_forearm_shared_dl);
	EXPORT_DL(mario_right_hand_cap);
	EXPORT_DL(mario_right_hand_cap_bottom_dl);
	EXPORT_DL(mario_right_hand_cap_dl);
	EXPORT_DL(mario_right_hand_cap_hand_position_dl);
	EXPORT_DL(mario_right_hand_cap_m_logo_dl);
	EXPORT_DL(mario_right_hand_cap_top_dl);
	EXPORT_DL(mario_right_hand_cap_wings);
	EXPORT_DL(mario_right_hand_cap_wings_end_dl);
	EXPORT_DL(mario_right_hand_cap_wings_half_1_dl);
	EXPORT_DL(mario_right_hand_cap_wings_half_2_dl);
	EXPORT_DL(mario_right_hand_cap_wings_intial_dl);
	EXPORT_DL(mario_right_hand_closed);
	EXPORT_DL(mario_right_hand_closed_dl);
	EXPORT_DL(mario_right_hand_open);
	EXPORT_DL(mario_right_hand_open_dl);
	EXPORT_DL(mario_right_hand_peace);
	EXPORT_DL(mario_right_hand_peace_shared_dl);
	EXPORT_DL(mario_right_leg_shared_dl);
	EXPORT_DL(mario_right_thigh);
	EXPORT_DL(mario_right_thigh_shared_dl);
	EXPORT_DL(mario_torso);
	EXPORT_DL(mario_torso_dl);
	EXPORT_DL(mario_tshirt_shared_dl);
	EXPORT_DL(mario_wings_half_1_dl);
	EXPORT_DL(mario_wings_half_1_unused_dl);
	EXPORT_DL(mario_wings_half_2_dl);
	EXPORT_DL(mario_wings_half_2_unused_dl);
	EXPORT_DL(mario_yellow_button_dl);
}
#endif

namespace sm64
{
	EXPORT_DL(bubble_seg4_dl_0401DD60);
	EXPORT_DL(bubble_seg4_dl_0401DDE0);
	EXPORT_DL(smoke_seg4_dl_040216A0);
	EXPORT_DL(smoke_seg4_dl_04021718);
	EXPORT_DL(smoke_seg4_dl_04021730);
	EXPORT_DL(smoke_seg4_dl_04021748);
	EXPORT_DL(smoke_seg4_dl_04021760);
	EXPORT_DL(smoke_seg4_dl_04021778);
	EXPORT_DL(smoke_seg4_dl_04021790);
	EXPORT_DL(smoke_seg4_dl_040217A8);
	EXPORT_DL(burn_smoke_seg4_dl_04022000);
	EXPORT_DL(burn_smoke_seg4_dl_04022028);
	EXPORT_DL(burn_smoke_seg4_dl_04022048);
	EXPORT_DL(burn_smoke_seg4_dl_04022070);
	EXPORT_DL(stomp_smoke_seg4_dl_04025148);
	EXPORT_DL(stomp_smoke_seg4_dl_04025190);
	EXPORT_DL(stomp_smoke_seg4_dl_040251C8);
	EXPORT_DL(stomp_smoke_seg4_dl_040251E0);
	EXPORT_DL(stomp_smoke_seg4_dl_040251F8);
	EXPORT_DL(stomp_smoke_seg4_dl_04025210);
	EXPORT_DL(stomp_smoke_seg4_dl_04025228);
	EXPORT_DL(stomp_smoke_seg4_dl_04025240);
	EXPORT_DL(stomp_smoke_seg4_dl_04025258);
	EXPORT_DL(stomp_smoke_seg4_dl_04025270);
	EXPORT_DL(stomp_smoke_seg4_dl_04025288);
	EXPORT_DL(stomp_smoke_seg4_dl_040252A0);
	EXPORT_DL(stomp_smoke_seg4_dl_040252B8);
	EXPORT_DL(stomp_smoke_seg4_dl_040252D0);
	EXPORT_DL(stomp_smoke_seg4_dl_040252E8);
	EXPORT_DL(stomp_smoke_seg4_dl_04025300);
	EXPORT_DL(water_waves_seg4_dl_04027358);
	EXPORT_DL(water_waves_seg4_dl_040273A0);
	EXPORT_DL(water_waves_seg4_dl_040273D8);
	EXPORT_DL(water_waves_seg4_dl_040273F0);
	EXPORT_DL(water_waves_seg4_dl_04027408);
	EXPORT_DL(water_waves_seg4_dl_04027420);
	EXPORT_DL(water_waves_seg4_dl_04027438);
	EXPORT_DL(sparkles_seg4_dl_0402A490);
	EXPORT_DL(sparkles_seg4_dl_0402A4F8);
	EXPORT_DL(sparkles_seg4_dl_0402A510);
	EXPORT_DL(sparkles_seg4_dl_0402A528);
	EXPORT_DL(sparkles_seg4_dl_0402A540);
	EXPORT_DL(sparkles_seg4_dl_0402A558);
	EXPORT_DL(sparkles_seg4_dl_0402A570);
	EXPORT_DL(water_splash_seg4_dl_040325C8);
	EXPORT_DL(water_splash_seg4_dl_04032640);
	EXPORT_DL(water_splash_seg4_dl_04032658);
	EXPORT_DL(water_splash_seg4_dl_04032670);
	EXPORT_DL(water_splash_seg4_dl_04032688);
	EXPORT_DL(water_splash_seg4_dl_040326A0);
	EXPORT_DL(water_splash_seg4_dl_040326B8);
	EXPORT_DL(water_splash_seg4_dl_040326D0);
	EXPORT_DL(water_splash_seg4_dl_040326E8);
	EXPORT_DL(white_particle_small_dl_begin);
	EXPORT_DL(white_particle_small_dl_end);
	EXPORT_DL(white_particle_small_dl);
	EXPORT_DL(white_particle_small_unused_dl);
	EXPORT_DL(sparkles_animation_seg4_dl_04035288);
	EXPORT_DL(sparkles_animation_seg4_dl_04035300);
	EXPORT_DL(sparkles_animation_seg4_dl_04035318);
	EXPORT_DL(sparkles_animation_seg4_dl_04035330);
	EXPORT_DL(sparkles_animation_seg4_dl_04035348);
	EXPORT_DL(sparkles_animation_seg4_dl_04035360);
}
