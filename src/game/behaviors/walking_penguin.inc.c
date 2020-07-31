// walking_penguin.c.inc

struct Struct8032FE4C movementList[] = {{60, 0, 6.0f, 1.0f}, {30, 3, 0.0f, 1.0f}, {30, 0, 12.0f, 2.0f}, {30, 3, 0.0f, 1.0f}, {30, 0, -6.0f, 1.0f}, {30, 3, 0.0f, 1.0f}, {-1, 0, 0.0f, 0.0f}};

s32 turnAround(void)
{
	o->oForwardVel = 0.0f;
	s_set_skelanime_speed(0, 1.0f);
	o->oAngleVelYaw = 0x400;

	o->oMoveAngleYaw += o->oAngleVelYaw / FRAME_RATE_SCALER_INV;
	if(o->oTimer == 31 * FRAME_RATE_SCALER_INV + (FRAME_RATE_SCALER_INV != 1 ? 1 : 0))
		return 1;
	else
		return 0;

	if(o->oMoveAngleYaw % 0x8000 == 0 && 0)
		return 1;
	else
	{
		if(o->oMoveAngleYaw == 0x001d6000)
			return 1;
		return 0;
	}
}

void bhv_walking_penguin_loop(void)
{
	f32 sp2C;
	f32 sp28;
	f32 sp24	= 100.0f;
	o->oAngleVelYaw = 0;
	s_enemybgcheck();

	switch(o->oAction)
	{
		case 0:
			if(o->oTimer == 0)
			{
				o->oWalkingPenguinMoveIndex   = 0;
				o->oWalkingPenguinMoveCounter = 0;
			}

			if(o->oWalkingPenguinMoveCounter < movementList[o->oWalkingPenguinMoveIndex].duration)
			{
				o->oWalkingPenguinMoveCounter++;
			}
			else
			{
				o->oWalkingPenguinMoveCounter = 0;
				o->oWalkingPenguinMoveIndex++;

				if(o->oWalkingPenguinMoveIndex >= sizeof(movementList) / sizeof(movementList[0]))
				{
					o->oWalkingPenguinMoveIndex = 0;
				}

				if(movementList[o->oWalkingPenguinMoveIndex].duration < 0)
				{
					o->oWalkingPenguinMoveIndex = 0;
				}
			}

			if(o->oPosX < 300.0f)
			{
				o->oAction++;
			}
			else
			{
				o->oForwardVel = movementList[o->oWalkingPenguinMoveIndex].forwardSpeed;

				s_set_skelanime_speed(movementList[o->oWalkingPenguinMoveIndex].unk1, movementList[o->oWalkingPenguinMoveIndex].animRate);
			}
			break;
		case 1:
			if(turnAround())
				o->oAction++;
			break;
		case 2:
			o->oForwardVel = 12.0f;
			s_set_skelanime_speed(0, 2.0f);

			if(o->oPosX > 1700.0f)
				o->oAction++;
			break;
		case 3:
			if(turnAround())
				o->oAction = 0;
			break;
	}
	s_enemymove(-78);

	if(sm64::config().camera().disableDistanceClip())
	{
		if(s_abs_f(o->oPosY - gMarioObject->oPosY) < 1000.0f)
		{
			play_penguin_walking_sound(1);
		}
	}
	else if(!s_erase_height(1000.0f))
	{
		play_penguin_walking_sound(1);
	}

	sp2C = o->oPosX + sins(0xDBB0) * 60.0f;
	sp28 = o->oPosZ + coss(0xDBB0) * 60.0f;
	sp2C += sp24 * sins(0x1BB0) / FRAME_RATE_SCALER_INV;
	sp28 += sp24 * coss(0x1BB0) / FRAME_RATE_SCALER_INV;
	o->oWalkingPenguinUnk100 = sp2C;
	o->oWalkingPenguinUnk104 = sp28;
}
