// tower_platform.c.inc

void bhv_wf_solid_tower_platform_loop(void)
{
	if(o->parentObj->oAction == 3)
	{
		s_remove_obj(o);
	}
}

void bhv_wf_elevator_tower_platform_loop(void)
{
	switch(o->oAction)
	{
		case 0:
			if(gMarioObject->platform == o)
			{
				o->oAction++;
			}
			break;
		case 1:
			objsound_level(SOUND_ENV_ELEVATOR1);
			if(o->oTimer > 140 * FRAME_RATE_SCALER_INV)
			{
				o->oAction++;
			}
			else
			{
				o->oPosY += 5.0f * FRAME_RATE_SCALER;
			}
			break;
		case 2:
			if(o->oTimer > 60 * FRAME_RATE_SCALER_INV)
			{
				o->oAction++;
			}
			break;
		case 3:
			objsound_level(SOUND_ENV_ELEVATOR1);
			if(o->oTimer > 140 * FRAME_RATE_SCALER_INV)
			{
				o->oAction = 0;
			}
			else
			{
				o->oPosY -= 5.0f * FRAME_RATE_SCALER;
			}
			break;
	}
	if(o->parentObj->oAction == 3)
	{
		s_remove_obj(o);
	}
}

void bhv_wf_sliding_tower_platform_loop(void)
{
	s32 duration = o->oPlatformTravelDistance / o->oPlatformCurrentVel / FRAME_RATE_SCALER;

	switch(o->oAction)
	{
		case 0:
			if(o->oTimer > duration)
			{
				o->oAction++;
			}

			o->oForwardVel = -o->oPlatformCurrentVel;
			break;
		case 1:
			if(o->oTimer > duration)
			{
				o->oAction = 0;
			}
			o->oForwardVel = o->oPlatformCurrentVel;
			break;
	}
	obj_compute_vel_xz();

	o->oPosX += o->oVelX * FRAME_RATE_SCALER;
	o->oPosZ += o->oVelZ * FRAME_RATE_SCALER;

	if(o->parentObj->oAction == 3)
	{
		s_remove_obj(o);
	}
}

void func_802AF9A4(s16 a, const BehaviorScript* beh)
{
	s16 yaw;
	struct Object* platform = s_makeobj_nowpos(o, a, beh);

	yaw			= o->oPlatformSpawnerUnkF4 * o->oPlatformSpawnerUnkFC + o->oPlatformSpawnerUnkF8;
	platform->oMoveAngleYaw = yaw;

	platform->oPosX += o->oPlatformSpawnerUnk100 * sins(yaw);
	platform->oPosY += 100 * o->oPlatformSpawnerUnkF4;
	platform->oPosZ += o->oPlatformSpawnerUnk100 * coss(yaw);

	platform->oPlatformTravelDistance = o->oPlatformSpawnerUnk104;
	platform->oPlatformCurrentVel	  = o->oPlatformSpawnerUnk108;

	o->oPlatformSpawnerUnkF4++;
}

void func_802AFAFC(void)
{
	UNUSED s32 unused = 8;

	o->oPlatformSpawnerUnkF4  = 0;
	o->oPlatformSpawnerUnkF8  = 0;
	o->oPlatformSpawnerUnkFC  = 0x2000;
	o->oPlatformSpawnerUnk100 = 704.0f;
	o->oPlatformSpawnerUnk104 = 380.0f;
	o->oPlatformSpawnerUnk108 = 3.0f;

	func_802AF9A4(MODEL_WF_TOWER_SQUARE_PLATORM, sm64::bhv::bhvWfSolidTowerPlatform());
	func_802AF9A4(MODEL_WF_TOWER_SQUARE_PLATORM, sm64::bhv::bhvWfSlidingTowerPlatform());
	func_802AF9A4(MODEL_WF_TOWER_SQUARE_PLATORM, sm64::bhv::bhvWfSolidTowerPlatform());
	func_802AF9A4(MODEL_WF_TOWER_SQUARE_PLATORM, sm64::bhv::bhvWfSlidingTowerPlatform());
	func_802AF9A4(MODEL_WF_TOWER_SQUARE_PLATORM, sm64::bhv::bhvWfSolidTowerPlatform());
	func_802AF9A4(MODEL_WF_TOWER_SQUARE_PLATORM, sm64::bhv::bhvWfSlidingTowerPlatform());
	func_802AF9A4(MODEL_WF_TOWER_SQUARE_PLATORM, sm64::bhv::bhvWfSolidTowerPlatform());
	func_802AF9A4(MODEL_WF_TOWER_SQUARE_PLATORM_ELEVATOR, sm64::bhv::bhvWfElevatorTowerPlatform());
}

void bhv_tower_platform_group_loop(void)
{
	f32 marioY	    = gMarioObject->oPosY;
	o->oDistanceToMario = dist_between_objects(o, gMarioObject);
	switch(o->oAction)
	{
		case 0:
			if(marioY > o->oHomeY - 1000.0f)
				o->oAction++;
			break;
		case 1:
			func_802AFAFC();
			o->oAction++;
			break;
		case 2:
			if(marioY < o->oHomeY - 1000.0f)
				o->oAction++;
			break;
		case 3:
			o->oAction = 0;
			break;
	}
}
