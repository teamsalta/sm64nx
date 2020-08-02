#ifndef OBJECT_HELPERS_H
#define OBJECT_HELPERS_H

#include "types.h"

// used for chain chomp and wiggler
struct ChainSegment
{
	f32 posX;
	f32 posY;
	f32 posZ;
	s16 pitch;
	s16 yaw;
	s16 roll;
};

#define WATER_SPLASH_FLAG_RAND_ANGLE 0x02
#define WATER_SPLASH_FLAG_RAND_OFFSET_XZ 0x04
#define WATER_SPLASH_FLAG_RAND_OFFSET_XYZ 0x08
#define WATER_SPLASH_FLAG_SET_Y_TO_WATER_LEVEL 0x20
#define WATER_SPLASH_FLAG_RAND_ANGLE_INCR_PLUS_8000 0x40
#define WATER_SPLASH_FLAG_RAND_ANGLE_INCR 0x80

// Call s_makeobj_initdata with this struct to spawn an object.
struct WaterSplashParams
{
	s16 flags; // spawn flags, see WATER_SPLASH_FLAG_* defines above
	s16 model;
	const BehaviorScript* behavior;
	s16 moveAngleRange; // only used for flags 0x40 & 0x80
	s16 moveRange;	    // only used for flags 0x04 & 0x08
	f32 randForwardVelOffset;
	f32 randForwardVelScale;
	f32 randYVelOffset;
	f32 randYVelScale;
	f32 randSizeOffset;
	f32 randSizeScale;
};

struct struct802A1230
{
	/*0x00*/ s16 unk00;
	/*0x02*/ s16 unk02;
};

struct Struct802A272C
{
	Vec3f vecF;
	Vec3s vecS;
};

// TODO: Field names
struct SpawnParticlesInfo
{
	/*0x00*/ s8 behParam;
	/*0x01*/ s8 count;
	/*0x02*/ u8 model;
	/*0x03*/ s8 offsetY;
	/*0x04*/ s8 forwardVelBase;
	/*0x05*/ s8 forwardVelRange;
	/*0x06*/ s8 velYBase;
	/*0x07*/ s8 velYRange;
	/*0x08*/ s8 gravity;
	/*0x09*/ s8 dragStrength;
	/*0x0C*/ f32 sizeBase;
	/*0x10*/ f32 sizeRange;
};

// extern ? D_80336610;
extern struct GraphNode** stageShapes;
// extern ? sLevelsWithRooms;
// extern ? sGrabReleaseState;
// extern ? sMrIParticleActions;
// extern ? D_8032F124;
// extern ? sMrIActions;
// extern ? D_8032F134;
// extern const char* D_803366B0;
// extern ? sKingBobombActions;
extern s16 gMarioShotFromCannon;
// extern ? sOpenedCannonActions;
// extern ? D_803366B4;
// extern ? D_803366BC;
// extern ? D_803366C4;
// extern ? sChuckyaActions;
// extern ? D_803366C8;
// extern ? D_8032F260;
// extern ? D_8032F273;
// extern ? D_8032F271;
// extern ? D_8032F270;
// extern ? D_8032F284;
// extern ? D_8032F294;
// extern ? sCoinInsideBooActions;
extern s16 D_8035FEE4;
// extern ? sGrindelThwompActions;
// extern ? sTumblingBridgeParams;
// extern ? sTumblingBridgeActions;
// extern ? sElevatorActions;
// extern ? D_8032F3CC;
// extern ? D_8032F3E0;
// extern ? D_8032F3F4;
// extern ? D_8032F3FC;
// extern ? sUkikiCageActions;
// extern ? D_8032F420;
// extern ? D_8032F430;
// extern ? D_8032F440;
// extern ? D_8032F450;
// extern ? sHeaveHoActions;
// extern ? D_8032F498;
// extern ? sJumpingBoxActions;
// extern ? D_8032F4B0;
// extern ? D_8035FF10;
// extern ? sBetaBooKeyActions;
// extern ? D_8032F4CC;
// extern ? sBulletBillActions;
// extern ? sBowserTailAnchorActions;
extern void* D_060576FC;
// extern ? speed_Y_data;
// extern ? speed_F_data;
// extern ? sBowserActions;
// extern ? kopa_sound;
// extern ? sFallingBowserPlatformActions;
// extern ? D_8032F738;
// extern ? D_8032F728;
// extern ? D_8032F754;
extern s8 dddStatus;
// extern ? D_8035FEE8;
// extern ? sCageUkikiPath;
// extern ? sUkikiActions;
// extern ? sUkikiSoundStates;
// extern ? D_803366D8;
// extern ? D_803366E4;
// extern ? D_803366F0;
// extern ? sRotatingCwFireBarsActions;
// extern ? D_8032F8E0;
// extern ? sToxBoxActions;
// extern ? sPiranhaPlantActions;
// extern ? D_8032FB4C;
// extern ? sBowserPuzzlePieceActions;
// extern ? sTuxiesMotherActions;
// extern ? sSmallPenguinActions;
// extern ? D_0301C2B0;
// extern ? D_0600E264;
// extern ? sFishActions;
// extern ? sFishGroupActions;
// extern ? sBirdChirpChirpActions;
// extern ? sCheepCheepActions;
// extern ? D_8032FC30;
// extern ? sExclamationBoxContents;
// extern ? itembox_modejmp;
// extern ? D_8032FCD8;
// extern ? D_8032FCE8;
// extern ? D_803366FC;
// extern ? D_8032FCF8;
// extern ? sTweesterActions;
// extern ? sCourtyardBooTripletPositions;
// extern ? sBooActions;
// extern ? D_8032FD14;
// extern ? sBooGivingStarActions;
// extern ? sBooWithCageActions;
// extern ? D_8032FD74;
// extern ? sWhompActions;
// extern ? D_8032FDAC;
// extern ? D_8032FDF4;
// extern ? D_8032FE3C;
// extern ? movementList;
// extern ? D_80336704;
// extern ? D_8033670C;

extern Gfx* CtrlHand(s32 run, UNUSED struct GraphNode* node, f32 mtx[4][4]);
extern Gfx* AlphaControl(s32 run, struct GraphNode* node, UNUSED void* context);

extern Gfx* ControlShapeAnime(s32 run, struct GraphNode* node, void* context);
extern Gfx* ControlSwitchMap(s32 run, struct GraphNode* node, void* context);

extern void s_calc_skeleton_glbpos(Mat4, struct Object*);
void s_copy_scale_matrix(struct Object*, Mat4, Mat4);
#ifdef __cplusplus
void s_calc_skeleton_glbmtx(Mat4& a0, const Mat4 a1, const Mat4 a2);
#endif
void s_modechange(struct Object*, const BehaviorScript*);
extern f32 s_distanceXZ_obj2obj(struct Object*, struct Object*);
extern f32 s_distance_obj2obj(struct Object*, struct Object*);
s16 object_lookat_yaw(struct Object* obj1, struct Object* obj2);
extern void obj_forward_vel_approach_upward(f32, f32);
extern s32 s_chase_speed(f32*, f32, f32);
extern f32 approach_f32_symmetric(f32, f32, f32);
extern s16 s_chase_angle(s16 arg0, s16 arg1, s16 arg2);
extern s32 s_chase_angleY(s16, s16);
extern s16 s_calc_targetangle(struct Object*, struct Object*);
extern s16 s_chase_obj_angle(struct Object*, struct Object*, s16, s16);
extern void s_set_skeleton(struct Object*, s16, s16, s16);
extern void s_set_world(struct Object*, s16, s16, s16);
extern void s_set_angle(struct Object*, s16, s16, s16);
extern struct Object* s_makeobj_absolute(struct Object*, s16, u32, const BehaviorScript*, s16, s16, s16, s16, s16, s16);
extern struct Object* s_makeobj_relative(struct Object* sp20, u32 sp24, const BehaviorScript* sp28, s16 sp2E, s16 sp32, s16 sp36, s16 sp3A, s16 sp3E, s16 sp42);
// extern ? s_makeobj_mother(?);
extern struct Object* s_makeobj_initdata(struct Object*, struct WaterSplashParams*);
extern struct Object* s_makeobj(struct Object*, s32, u32, const BehaviorScript*);
extern struct Object* s_makeobj_nowpos(struct Object*, s32, const BehaviorScript*);
extern struct Object* s_makeobj_effect(s16, f32, struct Object*, s32, const BehaviorScript*);
extern struct Object* s_makeobj_nowpos_scale(struct Object*, s32, const BehaviorScript*, f32);
// extern ? s_set_childpos(?);
extern struct Object* s_makeobj_chain(s16, s16, s16, s16, struct Object*, s32, const BehaviorScript*);
extern struct Object* s_makeobj_chain_scale(s16, s16, s16, s16, f32, struct Object*, s32, const BehaviorScript*);
// extern ? s_calc_speedXYZ(?);
extern void s_copy_animeposY(struct Object*, struct Object*);
extern void s_copy_worldXYZ_angleXYZ(struct Object*, struct Object*);
extern void s_copy_worldXYZ(struct Object*, struct Object*);
// extern ? s_copy_angleXYZ(?);
extern void s_copy_worldXYZmappos(struct Object*);
// extern ? s_set_animation(?);
void stRotatePoint(const Mat4, Vec3f&, const Vec3f);
void stBackPoint(const Mat4, Vec3f&, const Vec3f);
// extern ? s_scale_matrix(?);
void s_copy_scale(struct Object* toObj, struct Object* fromObj);
extern void stSetScale(struct Object* obj, f32 xScale, f32 yScale, f32 zScale);
extern void s_scale(struct Object*, f32);
extern void s_set_scale(f32);
extern void s_set_skelanimeNo(s32);
extern void s_set_skelanime_speed(u32, f32);
extern void stSetSkelAnimeNumber(s32 arg0);
void s_set_skeletonobj(struct Object* a0, struct Animation** a1, u32 a2);
// extern ? s_shapedispON(?);
extern void s_shapeON(void);
// extern ? s_shapedispOFF(?);
extern void s_shapeOFF(void);
extern void s_shape_disp(void);
extern void s_shape_hide(void);
extern void s_posoffset_mother(struct Object* MarioObj, f32, f32, f32);
// extern ? s_set_catchOFF(?);
extern void s_throw_object(void);
// extern ? s_drop_object(?);
extern void s_copy_animeangle(struct Object*);
u32 s_check_enemytype(const BehaviorScript* behavior);
extern struct Object* s_find_obj(const BehaviorScript*);
f32 s_distance_nearobj(const BehaviorScript*);
extern struct Object* s_search_nearobject(const BehaviorScript*, f32*);
Object* s_get_effectobj(const BehaviorScript* behavior, s16* dist);
extern struct Object* s_count_effect(void);
// extern ? s_count_obj(?);
extern s32 s_count_obj(const BehaviorScript* behavior);
struct Object* s_search_catch_enemy(const BehaviorScript*, f32);
// extern ? s_modereset(?);
void s_set_newmode(s32);
void s_chase_playerspeed(f32, f32);
void s_wait_anime(void);
extern void s_stop_animeend(void);
extern s32 s_check_animeend(void);
extern s32 s_check_animeend_2(void);
extern s32 s_check_animenumber(s32);
extern s32 s_check_animenumber_speed(s32, s32);
// extern ? s_check_animetable(?);
s32 s_check_playerjump(void);
s32 s_check_playercatch(void);
void s_set_jumpstart(f32, s32);
void s_mode_catch(s32, s32);
// extern ? obj_move_after_thrown_or_dropped(?);
void s_mode_throw(f32, f32, s32);
extern void s_mode_drop(void);
extern void s_change_shape(s32);
// extern ? mario_set_flag(?);
s32 obj_clear_interact_status_flag(s32);
extern void s_remove_obj(struct Object*);
void s_allOFF(void);
extern void s_hitOFF(void);
extern void s_hitON(void);
void s_hitON_obj(struct Object*);
void s_groundcheck(void);
struct Surface* s_groundcheck_ptr(void);
// extern ? calc_friction(?);
void s_calc_enemyspeedXZ(f32);
// extern ? obj_move_xz(?);
// extern ? obj_move_update_underwater_flags(?);
// extern ? obj_move_update_ground_air_flags(?);
// extern ? s_calc_speedY(?);
void s_moveobj(f32, f32, f32);
// extern ? s_bitcheck_clear(?);
// extern ? s_wall_revise(?);
extern s16 s_calc_dangle(s16, s16);
extern void s_calc_FspeedON(void);
extern void s_calc_YspeedON(void);
void s_calcobj_speedF(void);
f32 s_chase_set_speedY(f32, f32, f32, f32);
extern s32 s_hitcheck(struct Object*, struct Object*);
void s_change_pathname(const BehaviorScript*);
void s_rename_pathname(struct Object*, const BehaviorScript*);
extern s32 s_check_pathname(const BehaviorScript*);
s32 s_test_pathname(struct Object*, const BehaviorScript*);
f32 s_calc_playerscope(void);
extern f32 s_calc_enemyscope(void);
// extern ? s_calc_enemyscope(?);
// extern ? s_check_enemyscope(?);
extern void s_copy_initpos(void);
void s_reset_posspeed(void);
extern void s_effect_updown(f32);
void s_set_camerainfo(struct Object* obj, s32 cameraEvent);
// extern ? s_set_playerinfo(?);
void s_softspritemodeON(struct Object* a0);
void s_sethitbox(f32, f32);
void s_setdamagebox(f32, f32);
// extern ? spawn_object_loot_coins(?);
// extern ? s_make_escape_coin(?);
extern void s_makecoin(struct Object*, s32, f32);
void s_makecoin_playerpos(void);
// extern ? s_check_areaY(?);
// extern ? s_anime_percent(?);
// extern ? obj_detect_steep_floor(?);
s32 s_wallcheck(void);
// extern ? obj_update_floor(?);
// extern ? s_enemybgcheck_main(?);
extern void s_enemybgcheck(void);
extern void s_enemymove(s16);
// extern ? s_maparea_check(?);
void s_optionmove_XZ(void);
void s_optionmove_F(void);
// extern ? s_calc_relpos(?);
s16 s_calc_returnangle(void);
void s_copy_mapwork(struct Object*, struct Object*);
extern void s_rotate_vertex(struct Object*, s16, s16);
extern void s_create_matrix(struct Object*, s16, s16);
extern void s_set_objmatrix(struct Object*);
extern void s_calc_relative(struct Object*);
// extern ? s_remove_relative(?);
// extern ? s_move_angle(?);
void s_move_animeangle(void);
// extern ? s_anglesame(?);
extern s32 s_road_move(s32);
extern void s_clear_delayrecord(struct ChainSegment*);
extern f32 s_random_f(f32);
void s_random_scale(struct Object*, f32, f32);
extern void s_random_XYZ_offset(struct Object*, f32);
extern void s_random_XZ_offset(struct Object*, f32);
// extern ? func_802A297C(?);
void s_3Dmove(void);
void s_makeeffect_chiri(struct SpawnParticlesInfo* sp28);
extern s16 s_wall_rebound(void);

float scale_scaler(float n, float scale);

#endif /* OBJECT_HELPERS_H */
