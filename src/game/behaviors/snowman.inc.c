// snowman.c.inc

static struct ObjectHitbox sRollingSphereHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 210,
    /* height:            */ 350,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_snowmans_bottom_init(void)
{
	struct Object* sp34;

	o->oHomeX = o->oPosX;
	o->oHomeY = o->oPosY;
	o->oHomeZ = o->oPosZ;

	o->oGravity  = 10.0f;
	o->oFriction = 0.999f;
	o->oBuoyancy = 2.0f;

	o->oVelY		= 0;
	o->oForwardVel		= 0;
	o->oSnowmansBottomUnkF4 = 0.4f;

	sp34 = s_find_obj(sm64::bhv::bhvSnowmansHead());
	if(sp34 != NULL)
	{
		o->parentObj = sp34;
	}
	s_makeobj_absolute(o, 0, MODEL_NONE, sm64::bhv::bhvSnowmansBodyCheckpoint(), -402, 461, -2898, 0, 0, 0);
}

void func_802EFB2C(void)
{
	s_set_hitparam(o, &sRollingSphereHitbox);

	if((o->oInteractStatus & INT_STATUS_INTERACTED) != 0)
	{
		o->oInteractStatus = 0;
	}
}

void func_802EFB84(f32 f12)
{
	o->oFaceAnglePitch += (s16)(o->oForwardVel * (100.0f / f12)) / FRAME_RATE_SCALER_INV;
	o->oSnowmansBottomUnkF4 += o->oForwardVel * 1e-4 * FRAME_RATE_SCALER;

	if(o->oSnowmansBottomUnkF4 > 1.0)
		o->oSnowmansBottomUnkF4 = 1.0f;
}

void func_802EFC44(void)
{
	UNUSED s16 sp26;
	s32 sp20;
	UNUSED s16 sp1E;

	o->oPathedStartWaypoint = (Waypoint*)segmented_to_virtual(&ccm_seg7_trajectory_snowman);
	sp26			= ObjMoveEvent_noInc();
	sp20			= s_road_move(sp20);
	o->oSnowmansBottomUnkF8 = o->oPathedTargetYaw;
	o->oMoveAngleYaw	= s_chase_angle(o->oMoveAngleYaw, o->oSnowmansBottomUnkF8, 0x400 / FRAME_RATE_SCALER_INV);

	if(o->oForwardVel > 70.0)
		o->oForwardVel = 70.0f;

	if(sp20 == -1)
	{
		sp1E = (u16)o->oAngleToMario - (u16)o->oMoveAngleYaw;
		if(ShapeSameAngle(o->oMoveAngleYaw, o->oAngleToMario, 0x2000) == 1 && o->oSnowmansBottomUnk1AC == 1)
		{
			o->oSnowmansBottomUnkF8 = o->oAngleToMario;
		}
		else
		{
			o->oSnowmansBottomUnkF8 = o->oMoveAngleYaw;
		}
		o->oAction = 2;
	}
}

void func_802EFDA0(void)
{
	UNUSED s16 sp26;

	sp26 = ObjMoveEvent_noInc();
	if(o->oForwardVel > 70.0)
		o->oForwardVel = 70.0f;

	o->oMoveAngleYaw = s_chase_angle(o->oMoveAngleYaw, o->oSnowmansBottomUnkF8, 0x400 / FRAME_RATE_SCALER_INV);
	if(ObjApproach(o, -4230.0f, -1344.0f, 1813.0f, 300))
	{
		s_burneffect(0, 0, 70.0f);
		o->oMoveAngleYaw = atan2s(1813.0f - o->oPosZ, -4230.0f - o->oPosX);
		o->oVelY	 = 80.0f;
		o->oForwardVel	 = 15.0f;
		o->oAction	 = 3;

		o->parentObj->oAction = 2;
		o->parentObj->oVelY   = 100.0f;
		objsound(SOUND_OBJ_SNOWMAN_BOUNCE);
	}

	if(o->oTimer == 200 * FRAME_RATE_SCALER_INV)
	{
		Obj_reset(MODEL_CCM_SNOWMAN_BASE, sm64::bhv::bhvSnowmansBottom(), 3000);
		o->activeFlags = 0;
	}
}

void func_802EFF58(void)
{
	UNUSED s16 sp1E;

	sp1E = ObjMoveEvent_noInc();
	if((sp1E & 0x09) == 0x09)
	{
		o->oAction = 4;
		s_hitOFF();
	}

	if((sp1E & 0x01) != 0)
	{
		s_burneffect(0, 0, 70.0f);
		o->oPosX       = -4230.0f;
		o->oPosZ       = 1813.0f;
		o->oForwardVel = 0.0f;
	}
}

void bhv_snowmans_bottom_loop(void)
{
	s16 sp1E;

	switch(o->oAction)
	{
		case 0:
			if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 400) == 1 && CtrlPlayerDialog(1) == 2)
			{
				sp1E = cameraDemoStratMsgNum(CUTSCENE_DIALOG, o, DIALOG_110);
				if(sp1E)
				{
					o->oForwardVel = 10.0f;
					o->oAction     = 1;
					CtrlPlayerDialog(0);
				}
			}
			break;

		case 1:
			func_802EFC44();
			func_802EFB84(o->oSnowmansBottomUnkF4);
			objsound_level(SOUND_ENV_UNKNOWN2);
			break;

		case 2:
			func_802EFDA0();
			func_802EFB84(o->oSnowmansBottomUnkF4);
			objsound_level(SOUND_ENV_UNKNOWN2);
			break;

		case 3:
			func_802EFF58();
			break;

		case 4:
			s_player_slideout_RH(210.0f, 550);
			break;
	}

	func_802EFB2C();
	PlayerApproachOnOff(o, 8000);
	s_set_scale(o->oSnowmansBottomUnkF4);
	o->oGraphYOffset = o->oSnowmansBottomUnkF4 * 180.0f;
}

void bhv_snowmans_head_init(void)
{
	u8 sp37;
	s8 sp36;

	sp37 = BuGetStarFlag(activePlayerNo - 1, activeCourseNo - 1);
	sp36 = (o->oBehParams >> 24) & 0xFF;

	s_set_scale(0.7f);

	o->oGravity  = 5.0f;
	o->oFriction = 0.999f;
	o->oBuoyancy = 2.0f;

	if((sp37 & (1 << sp36)) && activeLevelNo != sp36 + 1)
	{
		s_makeobj_absolute(o, 0, MODEL_CCM_SNOWMAN_BASE, sm64::bhv::bhvBigSnowmanWhole(), -4230, -1344, 1813, 0, 0, 0);
		o->oPosX   = -4230.0f;
		o->oPosY   = -994.0f;
		o->oPosZ   = 1813.0f;
		o->oAction = 1;
	}
}

void bhv_snowmans_head_loop(void)
{
	UNUSED s16 sp1E;
	s16 sp1C;

	switch(o->oAction)
	{
		case 0:
			if(iwa_ObjMessage(&o->oSnowmansHeadUnkF4, DIALOG_109, 400.0f, 1))
				o->oAction = 1;
			break;

		case 1:
			break;

		case 2:
			sp1C = ObjMoveEvent_noInc();
			if(sp1C & 0x08)
				o->oAction = 3;
			break;

		case 3:
			ObjMoveEvent_noInc();
			if(o->oPosY < -994.0f)
			{
				o->oPosY   = -994.0f;
				o->oAction = 4;
				objsound(SOUND_OBJ_SNOWMAN_EXPLODE);
				Na_NazoClearBgm();
			}
			break;

		case 4:
			if(iwa_ObjMessage(&o->oSnowmansHeadUnkF4, DIALOG_111, 700.0f, 2))
			{
				s_kemuri();
				s_enemyset_star(-4700.0f, -1024.0f, 1890.0f);
				o->oAction = 1;
			}
			break;
	}

	s_player_slideout_RH(180.0f, 150.0f);
}

void bhv_snowmans_body_checkpoint_loop(void)
{
	if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 800))
	{
		o->parentObj->oSnowmansBottomUnk1AC++;
		o->activeFlags = 0;
	}

	if(o->parentObj->activeFlags == 0)
		o->activeFlags = 0;
}
