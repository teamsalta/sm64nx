#ifndef _OBJ_BEHAVIORS_H
#define _OBJ_BEHAVIORS_H

#include "types.h"
#include "object_helpers2.h"
#include "engine/surface_collision.h"

#include "levels/bob/header.h"

void iwa_StratInit(void);

s32 CoinStep(s16* collisionFlagsPtr);
void MovingCoinFlickerLoop(void);
void CoinCollected(void);
void bhv_moving_yellow_coin_init(void);
void bhv_moving_yellow_coin_loop(void);
void bhv_moving_blue_coin_init(void);
void bhv_moving_blue_coin_loop(void);
void bhv_blue_coin_sliding_jumping_init(void);
void func_802E540C(void);	       /* likely size */
void func_802E54DC(void);	       /* likely size */
void bhv_blue_coin_sliding_loop(void); /* likely size */
void bhv_blue_coin_jumping_loop(void); /* likely size */
void bhv_seaweed_init(void);
void bhv_seaweed_bundle_init(void);
void bhv_bobomb_init(void);
void func_802E5B7C(void);
void BobombExplodeLoop(void);
void CheckBobombInteractions(void);
void BobombPatrolLoop(void);
void BobombChaseMarioLoop(void);
void BobombLaunchedLoop(void);
void GenericBobombFreeLoop(void);
void StationaryBobombFreeLoop(void);
void BobombFreeLoop(void);
void BobombHeldLoop(void);
void BobombDroppedLoop(void);
void BobombThrownLoop(void);
void ObjRandomBlink(s32* blinkTimer);
void bhv_bobomb_loop(void);
void bhv_bobomb_fuse_smoke_init(void);
void bhv_bobomb_buddy_init(void);
void BobombBuddyIdleLoop(void);
void BobombBuddyCannonLoop(s16 arg0, s16 arg1);
void BobombBuddyTalkLoop(void);
void BobombBuddyTurnToTalkLoop(void);
void BobombBuddyActionLoop(void);
void bhv_bobomb_buddy_loop(void);
void bhv_cannon_closed_init(void);
void CannonTrapDoorOpeningLoop(void);
void bhv_cannon_closed_loop(void);
void bhv_whirlpool_init(void);
void func_802E70A8(void);
void WhirlpoolOrientGraph(void);
void bhv_whirlpool_loop(void);
void bhv_jet_stream_loop(void);
void bhv_homing_amp_init(void);
void bhv_homing_amp_loop(void);
void bhv_circling_amp_init(void);
void bhv_circling_amp_loop(void);
void s_butterflyInit(void);
void ButterflyWorldMove(s32 speed);
void ButterflyDirection(void);
void ButterflyStay(void);
void ButterflyTakeOff(void);
void ButterflyComeback(void);
void s_butterflyfunc(void);
void bhv_hoot_init(void);
f32 HootFindNextFloor(struct FloorGeometry** arg0, f32 arg1);
void HootFloorBounce(void);
void HootFreeStep(s16 arg0, s32 arg1);
void PlayerSetHootYaw(void);
void HootCarryStep(s32 arg0, UNUSED f32 arg1, UNUSED f32 arg2);
void HootSurfaceCollision(f32 arg0, UNUSED f32 arg1, f32 arg2);
void HootAscentLoop(f32 arg0, f32 arg1);
void HootActionLoop(void);
void HootTurnToHome(void);
void HootAwakeLoop(void);
void bhv_hoot_loop(void);
void bhv_beta_holdable_object_init(void); /* size */
void bhv_beta_holdable_object_loop(void); /* size */
void bhv_object_bubble_init(void);
void bhv_object_bubble_loop(void);
void bhv_object_water_wave_init(void);
void bhv_object_water_wave_loop(void);
void bhv_explosion_init(void);
void bhv_explosion_loop(void);
void bhv_bobomb_bully_death_smoke_init(void);
void bhv_bobomb_explosion_bubble_init(void);
void bhv_bobomb_explosion_bubble_loop(void);
void bhv_respawner_loop(void);
void Obj_reset(s32 arg0, const BehaviorScript* behToSpawn, s32 minSpawnDist);
void bhv_small_bully_init(void);
void bhv_big_bully_init(void);
void BullyCheckMarioCollision(void);
void BullyChaseMarioLoop(void);
void BullyKnockbackLoop(void);
void BullyBackUpLoop(void);
void BullyBackUpCheck(s16 arg0);
void PlayBullyStompingSound(void);
void BullyStep(void);
void BullySpawnCoin(void);
void BullyLavaDeath(void);
void bhv_bully_loop(void);
void BigBullySpawnMinion(s32 arg0, s32 arg1, s32 arg2, s16 arg3);
void bhv_big_bully_with_minions_init(void);
void BigBullyWithMinionsLavaDeath(void);
void bhv_big_bully_with_minions_loop(void);
f32 WaterRingCalcMarioDistInFront(void);
void WaterRingInit(void);
void bhv_jet_stream_water_ring_init(void);
void CheckWaterRingCollection(f32 avgScale, struct Object* ringManager);
void SetWaterRingScale(f32 avgScale);
void WaterRingCollectedLoop(void);
void JetStreamWaterRingNotCollectedLoop(void);
void bhv_jet_stream_water_ring_loop(void);
void Unknown802EB8A4(void); /* size */
void JetStreamRingSpawnerActiveLoop(void);
void bhv_jet_stream_ring_spawner_loop(void);
void bhv_manta_ray_water_ring_init(void);
void MantaRayWaterRingNotCollectedLoop(void);
void bhv_manta_ray_water_ring_loop(void);
void bhv_bowser_bomb_loop(void);
void bhv_bowser_bomb_explosion_loop(void);
void bhv_bowser_bomb_smoke_loop(void);
void bhv_celebration_star_init(void);
void CelebrationStarSpinAroundMarioLoop(void);
void CelebrationStarFaceCameraLoop(void);
void bhv_celebration_star_loop(void);
void bhv_celebration_star_sparkle_loop(void);
void bhv_star_key_collection_puff_spawner_loop(void);
void bhv_lll_drawbridge_spawner_loop(void);
void bhv_lll_drawbridge_loop(void);
void bhv_small_bomp_init(void);
void bhv_small_bomp_loop(void);
void bhv_large_bomp_init(void);
void bhv_large_bomp_loop(void);
void bhv_wf_sliding_platform_init(void);
void bhv_wf_sliding_platform_loop(void);
void bhv_moneybag_init(void);
void MoneybagCheckMarioCollision(void);
void MoneybagJump(s8 collisionFlags);
void MoneybagMoveAroundLoop(void);
void MoneybagReturnHomeLoop(void);
void MoneybagDisappearLoop(void);
void MoneybagDeathLoop(void);
void bhv_moneybag_loop(void);
void bhv_moneybag_hidden_loop(void);
void s_rail_ironball_init(void);
void iron_PlayerTouch(void);
void iron_ReadRailData(void);
void Ironball_Move(void);
void iron_ProgramInit(void);
void s_rail_ironball(void);
void s_gush_ironball_init(void);
void s_gush_ironball(void);
void s_gush_ironball2(void);
void s_ironball_init(void);
void s_ironball(void);
void s_ironball_normal_init(void);	    /* likely size */
void normal_ball_Move(void); /* likely size */
void s_ironball_normal_main(void);	    /* likely size */
void bhv_rr_cruiser_wing_init(void);
void bhv_rr_cruiser_wing_loop(void);
extern void s_enemyset_star(f32, f32, f32);

#endif /* _OBJ_BEHAVIORS_H */
