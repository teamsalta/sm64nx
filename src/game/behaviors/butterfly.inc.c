// butterfly.c.inc

void s_butterflyInit(void)
{
	stSetSkelAnimeNumber(1);

	o->oButterflyYPhase = Randomf() * 100.0f;
	o->header.gfx.unk38.setFrameRaw(Randomf() * 7.0f);
	o->oHomeX = o->oPosX;
	o->oHomeY = o->oPosY;
	o->oHomeZ = o->oPosZ;
}

// sp28 = speed

void ButterflyWorldMove(s32 speed)
{
	struct FloorGeometry* sp24;
	s16 yaw	   = o->oMoveAngleYaw;
	s16 pitch  = o->oMoveAnglePitch;
	s16 yPhase = o->oButterflyYPhase;
	f32 floorY;

	o->oVelX = sins(yaw) * (f32)speed;
	o->oVelY = sins(pitch) * (f32)speed;
	o->oVelZ = coss(yaw) * (f32)speed;

	o->oPosX += o->oVelX * FRAME_RATE_SCALER;
	o->oPosZ += o->oVelZ * FRAME_RATE_SCALER;

	if(o->oAction == BUTTERFLY_ACT_FOLLOW_MARIO)
		o->oPosY -= (o->oVelY + coss((s32)(yPhase * 655.36)) * 20.0f / 4.0f) * FRAME_RATE_SCALER;
	else
		o->oPosY -= o->oVelY * FRAME_RATE_SCALER;

	floorY = GroundCheck(o->oPosX, o->oPosY, o->oPosZ, &sp24);

	if(o->oPosY < floorY + 2.0f)
		o->oPosY = floorY + 2.0f;

	o->oButterflyYPhase++;
	if(o->oButterflyYPhase >= 101)
		o->oButterflyYPhase = 0;
}

void ButterflyDirection(void)
{
	gMarioObject->oPosX += 5 * o->oButterflyYPhase / (4 * FRAME_RATE_SCALER_INV);
	gMarioObject->oPosZ += 5 * o->oButterflyYPhase / (4 * FRAME_RATE_SCALER_INV);
	s_chase_obj_angle(o, gMarioObject, 16, 0x300 / FRAME_RATE_SCALER_INV);
	gMarioObject->oPosX -= 5 * o->oButterflyYPhase / (4 * FRAME_RATE_SCALER_INV);
	gMarioObject->oPosZ -= 5 * o->oButterflyYPhase / (4 * FRAME_RATE_SCALER_INV);

	gMarioObject->oPosY += (5 * o->oButterflyYPhase + 0x100) / (4 * FRAME_RATE_SCALER_INV);
	s_chase_obj_angle(o, gMarioObject, 15, 0x500 / FRAME_RATE_SCALER_INV);
	gMarioObject->oPosY -= (5 * o->oButterflyYPhase + 0x100) / (4 * FRAME_RATE_SCALER_INV);
}

void ButterflyStay(void)
{
	if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 1000))
	{
		stSetSkelAnimeNumber(0);

		o->oAction	 = BUTTERFLY_ACT_FOLLOW_MARIO;
		o->oMoveAngleYaw = gMarioObject->header.gfx.angle[1];
	}
}

void ButterflyTakeOff(void)
{
	ButterflyDirection();

	ButterflyWorldMove(7);

	if(!PlayerApproach(o->oHomeX, o->oHomeY, o->oHomeZ, 1200))
		o->oAction = BUTTERFLY_ACT_RETURN_HOME;
}

void ButterflyComeback(void)
{
	f32 homeDistX	 = o->oHomeX - o->oPosX;
	f32 homeDistY	 = o->oHomeY - o->oPosY;
	f32 homeDistZ	 = o->oHomeZ - o->oPosZ;
	s16 hAngleToHome = atan2s(homeDistZ, homeDistX);
	s16 vAngleToHome = atan2s(sqrtf(homeDistX * homeDistX + homeDistZ * homeDistZ), -homeDistY);

	o->oMoveAngleYaw   = s_chase_angle(o->oMoveAngleYaw, hAngleToHome, 0x800 / FRAME_RATE_SCALER_INV);
	o->oMoveAnglePitch = s_chase_angle(o->oMoveAnglePitch, vAngleToHome, 0x50 / FRAME_RATE_SCALER_INV);

	ButterflyWorldMove(7);

	if(homeDistX * homeDistX + homeDistY * homeDistY + homeDistZ * homeDistZ < 144.0f)
	{
		stSetSkelAnimeNumber(1);

		o->oAction = BUTTERFLY_ACT_RESTING;
		o->oPosX   = o->oHomeX;
		o->oPosY   = o->oHomeY;
		o->oPosZ   = o->oHomeZ;
	}
}

void s_butterflyfunc(void)
{
	switch(o->oAction)
	{
		case BUTTERFLY_ACT_RESTING:
			ButterflyStay();
			break;

		case BUTTERFLY_ACT_FOLLOW_MARIO:
			ButterflyTakeOff();
			break;

		case BUTTERFLY_ACT_RETURN_HOME:
			ButterflyComeback();
			break;
	}

	PlayerApproachOnOff(o, 3000);
}
