// cannon.c.inc

void bhv_cannon_base_unused_loop(void)
{
	o->oPosY += o->oVelY * FRAME_RATE_SCALER;
}

void ActionOpenedCannon0(void)
{
	if(o->oTimer == 0)
	{
		o->oInteractStatus = 0;
		o->oPosX	   = o->oHomeX;
		o->oPosY	   = o->oHomeY;
		o->oPosZ	   = o->oHomeZ;
		o->oMoveAnglePitch = 0;
		o->oMoveAngleYaw   = (s16)(o->oBehParams2ndByte << 8);
		o->oCannonUnkF4	   = 0;
		o->oCannonUnk10C   = 0;
		obj_enable_rendering();
		s_hitON();
	}
	if(o->oDistanceToMario < 500.0f)
	{
		s_hitON();
		obj_enable_rendering();
		if(o->oInteractStatus & INT_STATUS_INTERACTED && (!(o->oInteractStatus & INT_STATUS_TOUCHED_BOB_OMB))) // bob-omb explodes when it gets into a cannon
		{
			o->oAction	 = 4;
			o->oCannonUnk10C = 1;
			o->oCannonUnkF8	 = 1;
		}
		else
			o->oInteractStatus = 0;
	}
	else
	{
		s_hitOFF();
		obj_disable_rendering();
		o->oCannonUnk10C = 0;
	}
}

void ActionOpenedCannon4(void)
{
	if(o->oTimer == 0)
		objsound(SOUND_OBJ_CANNON1);
	o->oPosY += 5.0f * FRAME_RATE_SCALER;
	o->oPosX += (f32)((o->oTimer / 2 & 1) - 0.5) * 2 * FRAME_RATE_SCALER;
	o->oPosZ += (f32)((o->oTimer / 2 & 1) - 0.5) * 2 * FRAME_RATE_SCALER;
	if(o->oTimer > 67 * FRAME_RATE_SCALER_INV)
	{
		o->oPosX += (f32)((o->oTimer / 2 & 1) - 0.5) * 4 * FRAME_RATE_SCALER;
		o->oPosZ += (f32)((o->oTimer / 2 & 1) - 0.5) * 4 * FRAME_RATE_SCALER;
		o->oAction = 6;
	}
}

void ActionOpenedCannon6(void)
{
	if(o->oTimer == 0)
		objsound(SOUND_OBJ_CANNON2);
	if(o->oTimer < 4 * FRAME_RATE_SCALER_INV)
	{
		o->oPosX += (f32)((o->oTimer / 2 & 1) - 0.5) * 4.0f * FRAME_RATE_SCALER;
		o->oPosZ += (f32)((o->oTimer / 2 & 1) - 0.5) * 4.0f * FRAME_RATE_SCALER;
	}
	else
	{
		if(o->oTimer < 6 * FRAME_RATE_SCALER_INV)
		{
		}
		else
		{
			if(o->oTimer < 22 * FRAME_RATE_SCALER_INV)
			{
				o->oMoveAngleYaw = sins(o->oCannonUnkF4) * 0x4000 + ((s16)(o->oBehParams2ndByte << 8));
				o->oCannonUnkF4 += 0x400 * FRAME_RATE_SCALER;
			}
			else if(o->oTimer < 26 * FRAME_RATE_SCALER_INV)
			{
			}
			else
			{
				o->oCannonUnkF4 = 0;
				o->oAction	= 5;
			}
		}
	}
}

void ActionOpenedCannon5(void)
{
	if(o->oTimer == 0)
		objsound(SOUND_OBJ_CANNON3);

	if(o->oTimer < 4 * FRAME_RATE_SCALER_INV)
	{
	}
	else
	{
		if(o->oTimer < 20 * FRAME_RATE_SCALER_INV)
		{
			o->oCannonUnkF4 += 0x400 / FRAME_RATE_SCALER_INV;
			o->oMoveAnglePitch = sins(o->oCannonUnkF4) * (0x2000 / FRAME_RATE_SCALER_INV);
		}
		else if(o->oTimer < 25 * FRAME_RATE_SCALER_INV)
		{
		}
		else
			o->oAction = 1;
	}
}

void ActionOpenedCannon1(void)
{
	UNUSED s32 unused;
	s_hitOFF();
	obj_disable_rendering();
	o->oCannonUnk10C     = 0;
	gMarioShotFromCannon = 1;
}

void ActionOpenedCannon2(void)
{
	o->oAction = 3;
}

void ActionOpenedCannon3(void)
{
	UNUSED s32 unused;
	if(o->oTimer > 3 * FRAME_RATE_SCALER_INV)
		o->oAction = 0;
}

void (*sOpenedCannonActions[])(void) = {ActionOpenedCannon0, ActionOpenedCannon1, ActionOpenedCannon2, ActionOpenedCannon3, ActionOpenedCannon4, ActionOpenedCannon5, ActionOpenedCannon6};

u8 unused0EA1FC[] = {2, 0, 0, 0, 0, 0, 0, 0, 63, 128, 0, 0, 2, 0, 0, 0, 65, 32, 0, 0, 63, 128, 0, 0, 2, 0, 0, 0, 65, 160, 0, 0, 63, 128, 0, 0, 2, 0, 0, 0, 65, 160, 0, 0, 63, 128, 0, 0, 8, 0, 0, 0, 65, 32, 0, 0, 63, 128, 0, 0};

void bhv_cannon_base_loop(void)
{
	s_modejmp(sOpenedCannonActions);
	if(o->oCannonUnkF8)
		o->oCannonUnkF8++;
	o->oInteractStatus = 0;
}

void bhv_cannon_barrel_loop(void)
{
	struct Object* parent = o->parentObj;
	if(parent->header.gfx.node.flags & GRAPH_RENDER_ACTIVE)
	{
		obj_enable_rendering();
		copy_object_pos(o, o->parentObj);
		o->oMoveAngleYaw   = o->parentObj->oMoveAngleYaw;
		o->oFaceAnglePitch = o->parentObj->oMoveAnglePitch;
	}
	else
		obj_disable_rendering();
}
