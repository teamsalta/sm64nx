// cap.c.inc

static struct ObjectHitbox sCapHitbox = {
    /* interactType:      */ INTERACT_CAP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 80,
    /* hurtboxRadius:     */ 90,
    /* hurtboxHeight:     */ 90,
};

s32 hat_HitCheckEvent(void)
{
	s_set_hitparam(o, &sCapHitbox);
	if(o->oInteractStatus & INT_STATUS_INTERACTED)
	{
		o->activeFlags	   = 0;
		o->oInteractStatus = 0;
		return 1;
	}

	return 0;
}

void hat_RemoveCheck(void)
{
	if(o->oTimer > 300 * FRAME_RATE_SCALER_INV)
	{
		iwa_TimerRemove(o, 300 * FRAME_RATE_SCALER_INV);
	}
}

void HatLostCheck(void)
{
	if(sObjFloor == NULL)
		return;

	switch(sObjFloor->type)
	{
		case SURFACE_DEATH_PLANE:
			o->activeFlags = 0;
			break;

		case SURFACE_SHALLOW_QUICKSAND:
		case SURFACE_DEEP_QUICKSAND:
		case SURFACE_QUICKSAND:
			o->oAction     = 10;
			o->oForwardVel = 0.0f;
			break;

		case SURFACE_DEEP_MOVING_QUICKSAND:
		case SURFACE_SHALLOW_MOVING_QUICKSAND:
		case SURFACE_MOVING_QUICKSAND:
			o->oAction	 = 11;
			o->oMoveAngleYaw = (sObjFloor->force & 0xFF) << 8;
			o->oForwardVel	 = -((sObjFloor->force & 0xff00) >> 8) * 2 + 8;
			break;

		case SURFACE_INSTANT_QUICKSAND:
			o->oAction     = 12;
			o->oForwardVel = 0;
			break;

		case SURFACE_INSTANT_MOVING_QUICKSAND:
			o->oAction	 = 13;
			o->oMoveAngleYaw = (sObjFloor->force & 0xFF) << 8;
			o->oForwardVel	 = -((sObjFloor->force & 0xff00) >> 8) * 2 + 8;
			break;
	}
}

void itemhat_Goodby(void)
{
	switch(o->oAction)
	{
		case 10:
			if(o->oTimer < 10 * FRAME_RATE_SCALER_INV)
			{
				o->oGraphYOffset += -1.0f * FRAME_RATE_SCALER;
				o->oFaceAnglePitch = 0x2000;
			}
			break;

		case 11:
			if(o->oTimer < 10 * FRAME_RATE_SCALER_INV)
				o->oGraphYOffset += -3.0f * FRAME_RATE_SCALER;

			o->oFaceAnglePitch = 0x2000;
			break;

		case 12:
			o->oGraphYOffset += -1.0f * FRAME_RATE_SCALER;
			if(o->oTimer >= 21 * FRAME_RATE_SCALER_INV)
				o->activeFlags = 0;

			break;

		case 13:
			o->oGraphYOffset += -6.0f * FRAME_RATE_SCALER;
			if(o->oTimer >= 21 * FRAME_RATE_SCALER_INV)
				o->activeFlags = 0;

			o->oFaceAnglePitch = 0x2000;
			break;
	}

	HatLostCheck();
}

void s_itemhat_wing_init(void)
{
	o->oGravity  = 1.2f;
	o->oFriction = 0.999f;
	o->oBuoyancy = 0.9f;
	o->oOpacity  = 255;
}

void hat_ScaleEvent(void)
{
	o->oCapUnkF8 += 0x2000 / FRAME_RATE_SCALER_INV;
	o->header.gfx.scale[1] = coss(o->oCapUnkF8) * 0.3 + 0.7;

	if(o->oCapUnkF8 == 0x10000)
	{
		o->oCapUnkF8 = 0;
		o->oCapUnkF4 = 2;
	}
}

void itemhat_wing_Move(void)
{
	s16 sp1E;

	o->oFaceAngleYaw += o->oForwardVel * 128.0f * FRAME_RATE_SCALER;
	sp1E = ObjMoveEvent();
	if(sp1E & 0x01)
	{
		HatLostCheck();
		if(o->oVelY != 0.0f)
		{
			o->oCapUnkF4 = 1;
			o->oVelY     = 0.0f;
		}
	}

	if(o->oCapUnkF4 == 1)
		hat_ScaleEvent();
}

void s_itemhat_wing_main(void)
{
	switch(o->oAction)
	{
		case 0:
			itemhat_wing_Move();
			break;

		default:
			ObjMoveEvent();
			itemhat_Goodby();
			break;
	}

	if(o->oTimer > 20 * FRAME_RATE_SCALER_INV)
		s_hitON();

	hat_RemoveCheck();
	hat_HitCheckEvent();
}

void s_itemhat_metal_init(void)
{
	o->oGravity  = 2.4f;
	o->oFriction = 0.999f;
	o->oBuoyancy = 1.5f;
	o->oOpacity  = 0xFF;
}

void itemhat_metal_Move(void)
{
	s16 sp1E;

	o->oFaceAngleYaw += o->oForwardVel * 128.0f * FRAME_RATE_SCALER;
	sp1E = ObjMoveEvent();
	if(sp1E & 0x01)
		HatLostCheck();
}

void s_itemhat_metal_main(void)
{
	switch(o->oAction)
	{
		case 0:
			itemhat_metal_Move();
			break;

		default:
			ObjMoveEvent();
			itemhat_Goodby();
			break;
	}

	if(o->oTimer > 20 * FRAME_RATE_SCALER_INV)
		s_hitON();

	hat_HitCheckEvent();
	hat_RemoveCheck();
}

void s_itemhat_blow_init(void)
{
	o->oGravity  = 0.7f;
	o->oFriction = 0.89f;
	o->oBuoyancy = 0.9f;
	o->oOpacity  = 0xFF;

	BuSetHatPosition(o->oPosX, o->oPosY, o->oPosZ);
}

void hat_LostEvent(void)
{
	BuClrItemFlag(SAVE_FLAG_CAP_ON_GROUND);

	switch(activeCourseNo)
	{
		case COURSE_SSL:
			BuSetItemFlag(SAVE_FLAG_CAP_ON_KLEPTO);
			break;

		case COURSE_SL:
			BuSetItemFlag(SAVE_FLAG_CAP_ON_MR_BLIZZARD);
			break;

		case COURSE_TTM:
			BuSetItemFlag(SAVE_FLAG_CAP_ON_UKIKI);
			break;

		default:
			BuSetItemFlag(SAVE_FLAG_CAP_ON_KLEPTO);
			break;
	}
}

void itemhat_blow_Move(void)
{
	s16 sp1E;

	o->oFaceAngleYaw += o->oForwardVel * 128.0f * FRAME_RATE_SCALER;
	o->oFaceAnglePitch += o->oForwardVel * 80.0f * FRAME_RATE_SCALER;

	sp1E = ObjMoveEvent();
	if(sp1E & 0x01)
	{
		HatLostCheck();

		if(o->oVelY != 0.0f)
		{
			o->oCapUnkF4	   = 1;
			o->oVelY	   = 0.0f;
			o->oFaceAnglePitch = 0;
		}
	}

	if(o->oCapUnkF4 == 1)
		hat_ScaleEvent();
}

void s_itemhat_blow_main(void)
{
	switch(o->oAction)
	{
		case 0:
			itemhat_blow_Move();
			break;

		default:
			ObjMoveEvent();
			itemhat_Goodby();
			break;
	}

	if((s32)o->oForwardVel != 0)
		BuSetHatPosition(o->oPosX, o->oPosY, o->oPosZ);

	if(o->activeFlags == 0)
		hat_LostEvent();

	if(hat_HitCheckEvent() == 1)
		BuClrItemFlag(SAVE_FLAG_CAP_ON_GROUND);
}

void s_itemhat_erase_init(void)
{
	o->oGravity  = 1.2f;
	o->oFriction = 0.999f;
	o->oBuoyancy = 0.9f;
	o->oOpacity  = 150;
}
