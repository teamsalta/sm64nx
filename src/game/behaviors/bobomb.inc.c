// bobomb.c.inc
#define FUSE_TIMER (151 * FRAME_RATE_SCALER_INV)

static struct ObjectHitbox sBobombHitbox = {
    /* interactType:      */ INTERACT_GRABBABLE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 65,
    /* height:            */ 113,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_bobomb_init(void)
{
	o->oGravity	       = 2.5;
	o->oFriction	       = 0.8;
	o->oBuoyancy	       = 1.3;
	o->oInteractionSubtype = INT_SUBTYPE_KICKABLE;
}

void func_802E5B7C(void)
{
	if(((o->oBehParams >> 8) & 0x1) == 0)
	{
		obj_spawn_yellow_coins(o, 1);
		o->oBehParams = 0x100;
		set_object_respawn_info_bits(o, 1);
	}
}

void BobombExplodeLoop(void)
{
	struct Object* explosion;
	if(o->oTimer < 5 * FRAME_RATE_SCALER_INV)
	{
		s_set_scale(1.0 + powf((f32)o->oTimer / (5.0 * FRAME_RATE_SCALER), FRAME_RATE_SCALER));
	}
	else
	{
		explosion = s_makeobj_nowpos(o, MODEL_EXPLOSION, sm64::bhv::bhvExplosion());
		explosion->oGraphYOffset += 100.0f * FRAME_RATE_SCALER;

		func_802E5B7C();
		create_respawner(MODEL_BLACK_BOBOMB, sm64::bhv::bhvBobomb(), 3000);
		o->activeFlags = 0;
	}
}

void CheckBobombInteractions(void)
{
	s_set_hitparam(o, &sBobombHitbox);
	if((o->oInteractStatus & INT_STATUS_INTERACTED) != 0) /* bit 15 */
	{
		if((o->oInteractStatus & INTERACT_GRABBABLE) != 0) /* bit 1 */
		{
			o->oMoveAngleYaw = gMarioObject->header.gfx.angle[1];
			o->oForwardVel	 = 25.0;
			o->oVelY	 = 30.0;
			o->oAction	 = BOBOMB_ACT_LAUNCHED;
		}

		if((o->oInteractStatus & INTERACT_TEXT) != 0) /* bit 23 */
			o->oAction = BOBOMB_ACT_EXPLODE;

		o->oInteractStatus = 0;
	}

	if(s_fire_hitcheck(o) == 1)
		o->oAction = BOBOMB_ACT_EXPLODE;
}

void BobombPatrolLoop(void)
{
	UNUSED s8 filler[4];
	UNUSED s16 sp22;
	s16 collisionFlags;

	sp22	       = o->header.gfx.unk38.frame();
	o->oForwardVel = 5.0;

	collisionFlags = ObjMoveEvent();
	if((obj_return_home_if_safe(o, o->oHomeX, o->oHomeY, o->oHomeZ, 400) == 1) && (ShapeSameAngle(o->oMoveAngleYaw, o->oAngleToMario, 0x2000) == 1))
	{
		o->oBobombFuseLit = 1;
		o->oAction	  = BOBOMB_ACT_CHASE_MARIO;
	}
	obj_check_floor_death(collisionFlags, sObjFloor);
}

void BobombChaseMarioLoop(void)
{
	UNUSED u8 filler[4];
	s16 sp1a, collisionFlags;

	sp1a	       = o->header.gfx.unk38.incrementFrame();
	o->oForwardVel = 20.0;

	collisionFlags = ObjMoveEvent();

	if(sp1a == 5 || sp1a == 16)
		objsound(SOUND_OBJ_BOBOMB_WALK);

	s_chase_obj_angle(o, gMarioObject, 16, 0x800);
	obj_check_floor_death(collisionFlags, sObjFloor);
}

void BobombLaunchedLoop(void)
{
	s16 collisionFlags = 0;
	collisionFlags	   = ObjMoveEvent();
	if((collisionFlags & 0x1) == 1)
		o->oAction = BOBOMB_ACT_EXPLODE; /* bit 0 */
}

void GenericBobombFreeLoop(void)
{
	switch(o->oAction)
	{
		case BOBOMB_ACT_PATROL:
			BobombPatrolLoop();
			break;

		case BOBOMB_ACT_LAUNCHED:
			BobombLaunchedLoop();
			break;

		case BOBOMB_ACT_CHASE_MARIO:
			BobombChaseMarioLoop();
			break;

		case BOBOMB_ACT_EXPLODE:
			BobombExplodeLoop();
			break;

		case BOBOMB_ACT_LAVA_DEATH:
			if(obj_lava_death() == 1)
				create_respawner(MODEL_BLACK_BOBOMB, sm64::bhv::bhvBobomb(), 3000);
			break;

		case BOBOMB_ACT_DEATH_PLANE_DEATH:
			o->activeFlags = 0;
			create_respawner(MODEL_BLACK_BOBOMB, sm64::bhv::bhvBobomb(), 3000);
			break;
	}

	CheckBobombInteractions();

	if(o->oBobombFuseTimer >= FUSE_TIMER)
		o->oAction = BOBOMB_ACT_EXPLODE;
}

void StationaryBobombFreeLoop(void)
{
	switch(o->oAction)
	{
		case BOBOMB_ACT_LAUNCHED:
			BobombLaunchedLoop();
			break;

		case BOBOMB_ACT_EXPLODE:
			BobombExplodeLoop();
			break;

		case BOBOMB_ACT_LAVA_DEATH:
			if(obj_lava_death() == 1)
				create_respawner(MODEL_BLACK_BOBOMB, sm64::bhv::bhvBobomb(), 3000);
			break;

		case BOBOMB_ACT_DEATH_PLANE_DEATH:
			o->activeFlags = 0;
			create_respawner(MODEL_BLACK_BOBOMB, sm64::bhv::bhvBobomb(), 3000);
			break;
	}

	CheckBobombInteractions();

	if(o->oBobombFuseTimer >= FUSE_TIMER)
	{
		o->oAction = BOBOMB_ACT_EXPLODE;
	}
}

void BobombFreeLoop(void)
{
	if(o->oBehParams2ndByte == BOBOMB_BP_STYPE_GENERIC)
		GenericBobombFreeLoop();
	else
		StationaryBobombFreeLoop();
}

void BobombHeldLoop(void)
{
	o->header.gfx.node.flags |= 0x10; /* bit 4 */
	stSetSkelAnimeNumber(1);
	s_posoffset_mother(gMarioObject, 0, 60.0f, 100.0);

	o->oBobombFuseLit = 1;
	if(o->oBobombFuseTimer >= FUSE_TIMER)
	{
		//! Although the Bob-omb's action is set to explode when the fuse timer expires,
		//  BobombExplodeLoop() will not execute until the bob-omb's held state changes.
		//  This allows the Bob-omb to be regrabbed indefinitely.

		gMarioObject->oInteractStatus |= INTERACT_DAMAGE; /* bit 3 */
		o->oAction = BOBOMB_ACT_EXPLODE;
	}
}

void BobombDroppedLoop(void)
{
	s_mode_drop();

	o->header.gfx.node.flags &= ~0x10; /* bit 4 = 0 */
	stSetSkelAnimeNumber(0);

	o->oHeldState = 0;
	o->oAction    = BOBOMB_ACT_PATROL;
}

void BobombThrownLoop(void)
{
	s_throw_object();

	o->header.gfx.node.flags &= ~0x10; /* bit 4 = 0 */
	o->oHeldState = 0;
	o->oFlags &= ~0x8; /* bit 3 */
	o->oForwardVel = 25.0;
	o->oVelY       = 20.0;

	// const float frameSkips = 4.0f;
	// s_posoffset_mother(gMarioObject, 0.0f, 60.0f, 100.0f - (frameSkips / FRAME_RATE_SCALER * o->oForwardVel));

	o->oAction = BOBOMB_ACT_LAUNCHED;
}

// sp18 = blinkTimer

void ObjRandomBlink(s32* blinkTimer)
{
	if(*blinkTimer == 0)
	{
		if((s16)(Randomf() * 100.0f) == 0)
		{
			o->oAnimState = 1;
			*blinkTimer   = 1 * FRAME_RATE_SCALER_INV;
		}
	}
	else
	{
		(*blinkTimer)++;
		if(*blinkTimer >= 6 * FRAME_RATE_SCALER_INV)
			o->oAnimState = 0;

		if(*blinkTimer >= 11 * FRAME_RATE_SCALER_INV)
			o->oAnimState = 1;

		if(*blinkTimer >= 16 * FRAME_RATE_SCALER_INV)
		{
			o->oAnimState = 0;
			*blinkTimer   = 0;
		}
	}
}

void bhv_bobomb_loop(void)
{
	s8 dustPeriodMinus1;
	if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 4000) != 0)
	{
		switch(o->oHeldState)
		{
			case HELD_FREE:
				BobombFreeLoop();
				break;

			case HELD_HELD:
				BobombHeldLoop();
				break;

			case HELD_THROWN:
				BobombThrownLoop();
				break;

			case HELD_DROPPED:
				BobombDroppedLoop();
				break;
		}

		ObjRandomBlink(&o->oBobombBlinkTimer);

		if(o->oBobombFuseLit == 1)
		{
			if(o->oBobombFuseTimer >= 121 * FRAME_RATE_SCALER_INV)
				dustPeriodMinus1 = 1;
			else
				dustPeriodMinus1 = 7;

			if((dustPeriodMinus1 & o->oBobombFuseTimer) == 0) /* oBobombFuseTimer % 2 or oBobombFuseTimer % 8 */
				s_makeobj_nowpos(o, MODEL_SMOKE, sm64::bhv::bhvBobombFuseSmoke());

			objsound_level(SOUND_AIR_BOBOMB_LIT_FUSE);

			o->oBobombFuseTimer++;
		}
	}
}

void bhv_bobomb_fuse_smoke_init(void)
{
	o->oPosX += (s32)(Randomf() * 80.0f) - 40;
	o->oPosY += (s32)(Randomf() * 80.0f) + 60;
	o->oPosZ += (s32)(Randomf() * 80.0f) - 40;
	s_set_scale(1.2f);
}

void bhv_bobomb_buddy_init(void)
{
	o->oGravity	       = 2.5;
	o->oFriction	       = 0.8;
	o->oBuoyancy	       = 1.3;
	o->oInteractionSubtype = INT_SUBTYPE_NPC;
}

void BobombBuddyIdleLoop(void)
{
	UNUSED u8 filler[4];
	s16 sp1a		  = o->header.gfx.unk38.frame();
	UNUSED s16 collisionFlags = 0;

	o->oBobombBuddyPosXCopy = o->oPosX;
	o->oBobombBuddyPosYCopy = o->oPosY;
	o->oBobombBuddyPosZCopy = o->oPosZ;

	collisionFlags = ObjMoveEvent();

	if((sp1a == 5) || (sp1a == 16))
		objsound(SOUND_OBJ_BOBOMB_WALK);

	if(o->oDistanceToMario < 1000.0f)
		o->oMoveAngleYaw = s_chase_angle(o->oMoveAngleYaw, o->oAngleToMario, 0x140 / FRAME_RATE_SCALER_INV);

	if(o->oInteractStatus == INT_STATUS_INTERACTED)
		o->oAction = BOBOMB_BUDDY_ACT_TURN_TO_TALK;
}

/**
 * Function for the Bob-omb Buddy cannon guy.
 * dialogFirstText is the first dialogID called when Bob-omb Buddy
 * starts to talk to Mario to prepare the cannon(s) for him.
 * Then the camera goes to the nearest cannon, to play the "prepare cannon" cutscene
 * dialogSecondText is called after Bob-omb Buddy has the cannon(s) ready and
 * then tells Mario that is "Ready for blastoff".
 */
void BobombBuddyCannonLoop(s16 dialogFirstText, s16 dialogSecondText)
{
	struct Object* cannonClosed;
	s16 buddyText, cutscene;

	switch(o->oBobombBuddyCannonStatus)
	{
		case BOBOMB_BUDDY_CANNON_UNOPENED:
			buddyText = cutscene_object_with_dialog(CUTSCENE_DIALOG, o, dialogFirstText);
			if(buddyText != 0)
			{
				BuSetCannonFlag();
				cannonClosed = s_find_obj(sm64::bhv::bhvCannonClosed());
				if(cannonClosed != 0)
					o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_OPENING;
				else
					o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_STOP_TALKING;
			}
			break;

		case BOBOMB_BUDDY_CANNON_OPENING:
			cannonClosed = s_find_obj(sm64::bhv::bhvCannonClosed());
			cutscene     = cutscene_object(CUTSCENE_PREPARE_CANNON, cannonClosed);
			if(cutscene == -1)
				o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_OPENED;
			break;

		case BOBOMB_BUDDY_CANNON_OPENED:
			buddyText = cutscene_object_with_dialog(CUTSCENE_DIALOG, o, dialogSecondText);
			if(buddyText != 0)
				o->oBobombBuddyCannonStatus = BOBOMB_BUDDY_CANNON_STOP_TALKING;
			break;

		case BOBOMB_BUDDY_CANNON_STOP_TALKING:
			CtrlPlayerDialog(0);

			o->activeFlags &= ~0x20; /* bit 5 */
			o->oBobombBuddyHasTalkedToMario = BOBOMB_BUDDY_HAS_TALKED;
			o->oInteractStatus		= 0;
			o->oAction			= BOBOMB_BUDDY_ACT_IDLE;
			o->oBobombBuddyCannonStatus	= BOBOMB_BUDDY_CANNON_OPENED;
			break;
	}
}

void BobombBuddyTalkLoop(void)
{
	if(CtrlPlayerDialog(1) == 2)
	{
		o->activeFlags |= 0x20; /* bit 5 */

		switch(o->oBobombBuddyRole)
		{
			case BOBOMB_BUDDY_ROLE_ADVICE:
				if(cutscene_object_with_dialog(CUTSCENE_DIALOG, o, o->oBehParams2ndByte) != BOBOMB_BUDDY_BP_STYPE_GENERIC)
				{
					CtrlPlayerDialog(0);

					o->activeFlags &= ~0x20; /* bit 5 */
					o->oBobombBuddyHasTalkedToMario = BOBOMB_BUDDY_HAS_TALKED;
					o->oInteractStatus		= 0;
					o->oAction			= BOBOMB_BUDDY_ACT_IDLE;
				}
				break;

			case BOBOMB_BUDDY_ROLE_CANNON:
				if(activeCourseNo == COURSE_BOB)
					BobombBuddyCannonLoop(DIALOG_004, DIALOG_105);
				else
					BobombBuddyCannonLoop(DIALOG_047, DIALOG_106);
				break;
		}
	}
}

void BobombBuddyTurnToTalkLoop(void)
{
	s16 sp1e = o->header.gfx.unk38.frame();
	if((sp1e == 5) || (sp1e == 16))
		objsound(SOUND_OBJ_BOBOMB_WALK);

	o->oMoveAngleYaw = s_chase_angle(o->oMoveAngleYaw, o->oAngleToMario, 0x1000 / FRAME_RATE_SCALER_INV);
	if((s16)o->oMoveAngleYaw == (s16)o->oAngleToMario)
		o->oAction = BOBOMB_BUDDY_ACT_TALK;

	objsound(SOUND_ACTION_READ_SIGN);
}

void BobombBuddyActionLoop(void)
{
	switch(o->oAction)
	{
		case BOBOMB_BUDDY_ACT_IDLE:
			BobombBuddyIdleLoop();
			break;

		case BOBOMB_BUDDY_ACT_TURN_TO_TALK:
			BobombBuddyTurnToTalkLoop();
			break;

		case BOBOMB_BUDDY_ACT_TALK:
			BobombBuddyTalkLoop();
			break;
	}

	PlayerApproachOnOff(o, 3000);
}

void bhv_bobomb_buddy_loop(void)
{
	BobombBuddyActionLoop();

	ObjRandomBlink(&o->oBobombBuddyBlinkTimer);

	o->oInteractStatus = 0;
}
