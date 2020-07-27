// coin.c.inc

// sp18 = collisionFlagsPtr

static struct ObjectHitbox sMovingYellowCoinHitbox = {
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 100,
    /* height:            */ 64,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

static struct ObjectHitbox sMovingBlueCoinHitbox = {
    /* interactType:      */ INTERACT_COIN,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 5,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 100,
    /* height:            */ 64,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

s32 CoinStep(s16* collisionFlagsPtr)
{
	*collisionFlagsPtr = ObjMoveEvent();

	obj_check_floor_death(*collisionFlagsPtr, sObjFloor);

	if((*collisionFlagsPtr & 0x1) != 0 && (*collisionFlagsPtr & 0x8) == 0) /* bit 0, bit 3 */
	{
		objsound(SOUND_GENERAL_COIN_DROP);
		return 1;
	}

	return 0;
}

void MovingCoinFlickerLoop(void)
{
	s16 collisionFlags;

	CoinStep(&collisionFlags);
	iwa_TimerRemove(o, 0);
}

void CoinCollected(void)
{
	s_makeobj_nowpos(o, MODEL_SPARKLES, sm64::bhv::bhvGoldenCoinSparkles());
	o->activeFlags = 0;
}

void bhv_moving_yellow_coin_init(void)
{
	o->oGravity  = 3.0f;
	o->oFriction = 1.0f;
	o->oBuoyancy = 1.5f;

	s_set_hitparam(o, &sMovingYellowCoinHitbox);
}

void bhv_moving_yellow_coin_loop(void)
{
	s16 collisionFlags;

	switch(o->oAction)
	{
		case MOV_YCOIN_ACT_IDLE:
			CoinStep(&collisionFlags);

			if(o->oTimer < 10 * FRAME_RATE_SCALER_INV)
				s_hitOFF();
			else
				s_hitON();

			if(o->oTimer >= 301 * FRAME_RATE_SCALER_INV)
				o->oAction = 1;
			break;

		case MOV_YCOIN_ACT_BLINKING:
			MovingCoinFlickerLoop();
			break;

		case MOV_YCOIN_ACT_LAVA_DEATH:
			o->activeFlags = 0;
			break;

		case MOV_YCOIN_ACT_DEATH_PLANE_DEATH:
			o->activeFlags = 0;
			break;
	}

	if((o->oInteractStatus & INT_STATUS_INTERACTED) != 0) /* bit 15 */
	{
		CoinCollected();
		o->oInteractStatus = 0;
	}
}

void bhv_moving_blue_coin_init(void)
{
	o->oGravity  = 5.0f;
	o->oFriction = 1.0f;
	o->oBuoyancy = 1.5f;

	s_set_hitparam(o, &sMovingBlueCoinHitbox);
}

void bhv_moving_blue_coin_loop(void)
{
	s16 collisionFlags;

	switch(o->oAction)
	{
		case MOV_BCOIN_ACT_STILL:
			if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 1500) != 0)
			{
				o->oAction = MOV_YCOIN_ACT_BLINKING;
			}
			break;

		case MOV_BCOIN_ACT_MOVING:
			collisionFlags = ObjMoveEvent();
			if((collisionFlags & 0x1) != 0) /* bit 0 */
			{
				o->oForwardVel += 25.0f * FRAME_RATE_SCALER;
				if((collisionFlags & 0x8) == 0)
					objsound(SOUND_GENERAL_COIN_DROP); /* bit 3 */
			}
			else
				o->oForwardVel *= 0.98;

			if(o->oForwardVel > 75.0)
				o->oForwardVel = 75.0f;

			iwa_TimerRemove(o, 600);
			break;
	}

	if((o->oInteractStatus & INT_STATUS_INTERACTED) != 0) /* bit 15 */
	{
		CoinCollected();
		o->oInteractStatus = 0;
	}
}

void bhv_blue_coin_sliding_jumping_init(void)
{
	o->oGravity  = 3.0;
	o->oFriction = 0.98;
	o->oBuoyancy = 1.5;

	s_set_hitparam(o, &sMovingBlueCoinHitbox);
}

void func_802E540C(void)
{
	s16 collisionFlags;

	o->oForwardVel	 = 15.0;
	o->oMoveAngleYaw = o->oAngleToMario + 0x8000;

	if(CoinStep(&collisionFlags) != 0)
		o->oVelY += 18.0f * FRAME_RATE_SCALER;
	if((collisionFlags & 0x2) != 0)
		o->oAction = 3; /* bit 1 */

	if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 1000) == 0)
		o->oAction = 2;
}

void func_802E54DC(void)
{
	s16 collisionFlags;

	CoinStep(&collisionFlags);

	if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 500) == 1)
	{
		o->oAction = MOV_YCOIN_ACT_BLINKING;
	}

	if(o->oTimer >= 151 * FRAME_RATE_SCALER_INV)
		o->oAction = 3;
}

void bhv_blue_coin_sliding_loop(void)
{
	s16 collisionFlags;

	switch(o->oAction)
	{
		case 0:
			if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 500) == 1)
			{
				o->oAction = MOV_YCOIN_ACT_BLINKING;
			}

			PlayerApproachOnOff(o, 3000);
			break;

		case 1:
			func_802E540C();
			break;

		case 2:
			func_802E54DC();
			PlayerApproachOnOff(o, 3000);
			break;

		case 3:
			CoinStep(&collisionFlags);
			if(o->oTimer >= 61 * FRAME_RATE_SCALER_INV)
				o->oAction = 4;
			break;

		case 4:
			MovingCoinFlickerLoop();
			break;

		case 100:
			o->activeFlags = 0;
			break;

		case 101:
			o->activeFlags = 0;
			break;
	}

	if((o->oInteractStatus & INT_STATUS_INTERACTED) != 0) /* bit 15 */
	{
		CoinCollected();
		o->oInteractStatus = 0;
	}
}

void bhv_blue_coin_jumping_loop(void)
{
	s16 collisionFlags;

	switch(o->oAction)
	{
		case 0:
			if(o->oTimer == 0)
			{
				s_hitOFF();
				o->oVelY = 50.0;
			}

			ObjMoveEvent();

			if(o->oTimer == 15 * FRAME_RATE_SCALER_INV)
			{
				s_hitON();
				o->oAction = MOV_YCOIN_ACT_BLINKING;
			}
			break;

		case 1:
			func_802E540C();
			break;

		case 2:
			func_802E54DC();
			PlayerApproachOnOff(o, 3000);
			break;

		case 3:
			CoinStep(&collisionFlags);
			if(o->oTimer >= 61 * FRAME_RATE_SCALER_INV)
				o->oAction = 4;
			break;

		case 4:
			MovingCoinFlickerLoop();
			break;
	}

	if((o->oInteractStatus & INT_STATUS_INTERACTED) != 0) /* bit 15 */
	{
		CoinCollected();
		o->oInteractStatus = 0;
	}
}
