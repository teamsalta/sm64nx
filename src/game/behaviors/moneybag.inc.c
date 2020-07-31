// moneybag.c.inc

static struct ObjectHitbox sMoneybagHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 120,
    /* height:            */ 60,
    /* hurtboxRadius:     */ 100,
    /* hurtboxHeight:     */ 50,
};

static struct ObjectHitbox sMoneybagHiddenHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 120,
    /* height:            */ 60,
    /* hurtboxRadius:     */ 100,
    /* hurtboxHeight:     */ 50,
};

void bhv_moneybag_init(void)
{
	o->oGravity  = 3.0f;
	o->oFriction = 1.0f;
	o->oBuoyancy = 2.0f;
	stSetSkelAnimeNumber(0);
	o->oOpacity = 0;
}

void MoneybagCheckMarioCollision(void)
{
	s_set_hitparam(o, &sMoneybagHitbox);

	if(o->oInteractStatus & INT_STATUS_INTERACTED) /* bit 15 */
	{
		if(o->oInteractStatus & INT_STATUS_ATTACKED_MARIO) /* bit 13 */
		{
			o->oMoveAngleYaw = o->oAngleToMario + 0x8000;
			o->oVelY	 = 30.0f;
		}

		if(o->oInteractStatus & INT_STATUS_WAS_ATTACKED) /* bit 14 */
		{
			o->oAction = MONEYBAG_ACT_DEATH;
		}

		o->oInteractStatus = 0;
	}
}

// sp20 = collisionFlags

void MoneybagJump(s8 collisionFlags)
{
	s16 animFrame = o->header.gfx.unk38.frame();

	switch(o->oMoneybagJumpState)
	{
		case MONEYBAG_JUMP_PREPARE:
			stSetSkelAnimeNumber(1);
			if(animFrame == 5)
			{
				o->oForwardVel = 20.0f;
				o->oVelY       = 40.0f;
			}

			if(s_check_animeend() == 1)
			{
				o->oMoneybagJumpState = MONEYBAG_JUMP_JUMP;
				objsound(SOUND_GENERAL_BOING2_LOWPRIO);
			}
			break;

		case MONEYBAG_JUMP_JUMP:
			stSetSkelAnimeNumber(2);

			if((collisionFlags & 1) == 1) /* bit 0 */
			{
				o->oForwardVel	      = 0;
				o->oVelY	      = 0;
				o->oMoneybagJumpState = MONEYBAG_JUMP_LANDING;
			}
			break;

		case MONEYBAG_JUMP_JUMP_AND_BOUNCE:
			stSetSkelAnimeNumber(3);

			if(s_check_animeend() == 1)
				o->oMoneybagJumpState = MONEYBAG_JUMP_LANDING;
			break;

		case MONEYBAG_JUMP_WALK_AROUND:
			stSetSkelAnimeNumber(4);
			o->oForwardVel = 10.0f;

			if(o->oTimer >= 61 * FRAME_RATE_SCALER_INV)
			{
				o->oMoneybagJumpState = MONEYBAG_JUMP_LANDING;
				o->oForwardVel	      = 0;
				o->header.gfx.unk38.setFrameRaw(0);
			}
			break;

		case MONEYBAG_JUMP_WALK_HOME:
			stSetSkelAnimeNumber(4);
			o->oForwardVel = 5.0f;
			break;
	}
}

void MoneybagMoveAroundLoop(void)
{
	s16 collisionFlags;

	obj_return_and_displace_home(o, o->oHomeX, o->oHomeY, o->oHomeZ, 200);

	collisionFlags = ObjMoveEvent();

	if(((collisionFlags & OBJ_COL_FLAGS_LANDED) == OBJ_COL_FLAGS_LANDED) && (o->oMoneybagJumpState == MONEYBAG_JUMP_LANDING))
	{
		if((s32)(Randomf() * 6.0f) == 1)
		{
			o->oMoneybagJumpState = MONEYBAG_JUMP_WALK_AROUND;
			o->oTimer	      = 0;
		}
		else
			o->oMoneybagJumpState = MONEYBAG_JUMP_PREPARE;
	}

	MoneybagJump(collisionFlags);
	MoneybagCheckMarioCollision();

	if(!PlayerApproach(o->oHomeX, o->oHomeY, o->oHomeZ, 800) && ((collisionFlags & 0x9) == 9))
		o->oAction = MONEYBAG_ACT_RETURN_HOME;
}

void MoneybagReturnHomeLoop(void)
{
	s16 collisionFlags;
	f32 sp28	 = o->oHomeX - o->oPosX;
	f32 sp24	 = o->oHomeZ - o->oPosZ;
	s16 sp22	 = atan2s(sp24, sp28);
	o->oMoveAngleYaw = s_chase_angle(o->oMoveAngleYaw, sp22, 0x800 / FRAME_RATE_SCALER_INV);

	collisionFlags = ObjMoveEvent();
	if(((collisionFlags & OBJ_COL_FLAGS_LANDED) == OBJ_COL_FLAGS_LANDED) && (o->oMoneybagJumpState == MONEYBAG_JUMP_LANDING))
		o->oMoneybagJumpState = MONEYBAG_JUMP_WALK_HOME;

	MoneybagJump(collisionFlags);
	MoneybagCheckMarioCollision();

	if(is_point_close_to_object(o, o->oHomeX, o->oHomeY, o->oHomeZ, 100))
	{
		s_makeobj_nowpos(o, MODEL_YELLOW_COIN, sm64::bhv::bhvMoneybagHidden());
		objsound(SOUND_GENERAL_VANISH_SFX);
		stSetSkelAnimeNumber(0);
		o->oAction	      = MONEYBAG_ACT_DISAPPEAR;
		o->oMoneybagJumpState = MONEYBAG_JUMP_LANDING;
	}

	if(PlayerApproach(o->oHomeX, o->oHomeY, o->oHomeZ, 800) == 1)
	{
		o->oAction	      = MONEYBAG_ACT_MOVE_AROUND;
		o->oMoneybagJumpState = MONEYBAG_JUMP_LANDING;
	}
}

void MoneybagDisappearLoop(void)
{
	o->oOpacity -= 6 / FRAME_RATE_SCALER_INV;
	if(o->oOpacity < 0)
	{
		o->oOpacity    = 0;
		o->activeFlags = 0;
	}
}

void MoneybagDeathLoop(void)
{
	if(o->oTimer == 1 * FRAME_RATE_SCALER_INV)
	{
		obj_spawn_yellow_coins(o, 5);
		obj_remove_sound(SOUND_GENERAL_SPLATTERING);
		s_kemuri();
		o->activeFlags = 0;
	}
}

void bhv_moneybag_loop(void)
{
	switch(o->oAction)
	{
		case MONEYBAG_ACT_APPEAR:
		case MONEYBAG_ACT_UNUSED_APPEAR:
			o->oOpacity += 12 / FRAME_RATE_SCALER_INV;
			if(o->oOpacity >= 256)
			{
				o->oOpacity		  = 255;
				o->parentObj->activeFlags = 0;
				o->oAction		  = MONEYBAG_ACT_MOVE_AROUND;
			}
			break;

		case MONEYBAG_ACT_MOVE_AROUND:
			MoneybagMoveAroundLoop();
			if(o->oTimer >= 31 * FRAME_RATE_SCALER_INV)
				s_hitON();
			break;

		case MONEYBAG_ACT_RETURN_HOME:
			MoneybagReturnHomeLoop();
			break;

		case MONEYBAG_ACT_DISAPPEAR:
			MoneybagDisappearLoop();
			break;

		case MONEYBAG_ACT_DEATH:
			MoneybagDeathLoop();
			break;
	}
}

void bhv_moneybag_hidden_loop(void)
{
	s_set_hitparam(o, &sMoneybagHiddenHitbox);

	switch(o->oAction)
	{
		case FAKE_MONEYBAG_COIN_ACT_IDLE:
			if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 400))
			{
				s_makeobj_nowpos(o, MODEL_MONEYBAG, sm64::bhv::bhvMoneybag());
				objsound(SOUND_GENERAL_VANISH_SFX);
				o->oAction = FAKE_MONEYBAG_COIN_ACT_TRANSFORM;
			}
			break;

		case FAKE_MONEYBAG_COIN_ACT_TRANSFORM:
			break;
	}

	o->oInteractStatus = 0;
}
