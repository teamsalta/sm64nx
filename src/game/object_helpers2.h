#ifndef OBJECT_HELPERS2_H
#define OBJECT_HELPERS2_H

#include "types.h"

// TODO: join this together with object_helpers.h

#define WAYPOINT_FLAGS_END -1
#define WAYPOINT_FLAGS_INITIALIZED 0x8000
#define WAYPOINT_MASK_00FF 0x00FF
#define WAYPOINT_FLAGS_PLATFORM_ON_TRACK_PAUSE 3

#define PATH_NONE 0
#define PATH_REACHED_END -1
#define PATH_REACHED_WAYPOINT 1

struct GraphNode_802A45E4
{
	/*0x00*/ s8 filler0[0x18 - 0x00];
	/*0x18*/ s16 unk18;
	/*0x1A*/ s16 unk1A;
	/*0x1C*/ s16 unk1C;
	/*0x1E*/ s16 unk1E;
	/*0x20*/ s16 unk20;
	/*0x22*/ s16 unk22;
};

extern void s_set_hitparam(struct Object* obj, struct ObjectHitbox* arg1);
s32 s_sign_d(s32);
extern f32 s_abs_f(f32);
extern s32 s_abs_d(s32 a0);
s32 s_flash_shape(s32 a0, s32 a1);
s32 s_checkplayer_hipaatack(void);
extern void s_kemuri(void);
extern void s_kemuri_sound(s32 sp18);
void s_player_slideout(f32);
void s_player_slideout_RH(f32 sp20, f32 sp24);
// extern ? s_samplesmoke_main(?);
s32 s_datamove_init(s8*);
s32 s_datamove_next(void);
// extern ? s_calc_searchangle(?);
extern void s_scale_timezoom(s32, s32, f32, f32);
void s_debug_position(void);
extern s32 s_rideon_player(void);
// extern ? s_buruburu(?);
s32 s_gatan(s32);
void s_modejmp(void (*[])(void));
// extern ? s_make_enemystar_entry(?);
// extern ? s_make_enemystar(?);
s32 s_index_bit(s32);
s32 s_enemy_areaout(void);
s32 s_check_playerslow(s32);
s32 s_check_chartable(s8, s8*);
extern void s_areastage_init(void); // 802A3978
extern void s_areastage_main(void);
s32 s_hit_remove_enemy(struct ObjectHitbox*, s32, s32);
void s_burn_remove(f32 sp18, s32 sp1C);
void s_set_shapeinfo(struct Object*, const void*);
void s_wallreverse(void);
s32 s_erase_height(f32);
extern Gfx* CtrlCoord(s32 run, struct GraphNode* node, UNUSED f32 mtx[4][4]);
// extern ? dbCtrlCoord(?);
s32 s_check_shapehide(struct Object*);
extern void s_begin_demomode(void);
extern void s_end_demomode(void);
void s_begin_enemydemo(s32);
void s_end_enemydemo(s32);
s32 s_hitcheck_message_entry(f32, f32, s32);
extern s32 s_hitcheck_message(f32 sp18, f32 sp1C);
// extern ? obj_end_dialog(?);
s32 s_call_enemydemo(s32 arg0, s32 dialogFlags, s32 dialogID, s32 unused);
s32 s_call_talkdemo(s32 arg0, s32 dialogFlags, s32 cutsceneTable, s32 dialogID);
s32 s_check_shapename(u16);
extern void s_set_mtxmode(void);
// extern ? s_boxcheck(?);
void s_call_Viewshake(s32 shake);
extern s32 s_fire_hitcheck(struct Object* obj);
s32 s_block_hitcheck(void);
void s_copy_actorcode(struct Object*, struct Object*);
void s_setanime_start(s32, s32);
s32 s_setanime_endcheck(s32);
void s_setanime_endstop(s32);
s32 s_check_playercarry(void);
s32 key_gacha(void);
// extern ? s_walk_sound(?);
// extern ? s_demobegin(?);
extern void s_demoend(void);
s32 s_pl_hitcheck(void);
void s_make_bluecoin(void);

void sv2_enemyset_star(f32 f12, f32 f14, f32 a2, f32 a3);

#endif /* OBJECT_HELPERS2_H */
