// manta_ray.c.inc

// TODO: these are likely Waypoint structs
static u16 D_803316A8[] = {0x0000, 0xEE6C, 0xFA9C, 0xFFD8, 0x0001, 0xEFE8, 0xF740, 0x02E4, 0x0002, 0xF330, 0xF3F8, 0x0410, 0x0003, 0xF740, 0xF308, 0x02D0, 0x0004,
			   0xF8D0, 0xF3BC, 0xFEE8, 0x0005, 0xF6F0, 0xF650, 0xFBB4, 0x0006, 0xF36C, 0xF9C0, 0xFAB0, 0x0007, 0xEFAC, 0xFC04, 0xFBF0, 0xFFFF, 0x0000};

static struct ObjectHitbox sMantaRayHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 3,
    /* numLootCoins:      */ 0,
    /* radius:            */ 210,
    /* height:            */ 60,
    /* hurtboxRadius:     */ 200,
    /* hurtboxHeight:     */ 50,
};

void bhv_manta_ray_init(void)
{
	struct Object* sp1C;
	sp1C	     = s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvMantaRayRingManager());
	o->parentObj = sp1C;
	s_set_hitparam(o, &sMantaRayHitbox);
	s_set_scale(2.5f);
}

void func_802F5E20(void)
{
	s16 sp1E;
	s32 sp18;

	sp1E				    = o->header.gfx.unk38.frame();
	gCurrentObject->oPathedWaypointsS16 = &D_803316A8;
	sp18				    = s_road_move(0);
	o->oMantaUnkF8			    = o->oPathedTargetYaw;
	o->oMantaUnkF4			    = o->oPathedTargetPitch;
	o->oForwardVel			    = 10.0f;
	o->oMoveAngleYaw		    = s_chase_angle(o->oMoveAngleYaw, o->oMantaUnkF8, 0x80 / FRAME_RATE_SCALER_INV);
	o->oMoveAnglePitch		    = s_chase_angle(o->oMoveAnglePitch, o->oMantaUnkF4, 0x80 / FRAME_RATE_SCALER_INV);
	if((s16)o->oMantaUnkF8 != (s16)o->oMoveAngleYaw)
	{
		o->oMoveAngleRoll -= 91;

		if(o->oMoveAngleRoll < -5461.3332)
			o->oMoveAngleRoll = -0x4000 / 3;
	}
	else
	{
		o->oMoveAngleRoll += 91;

		if(o->oMoveAngleRoll > 5461.3332)
			o->oMoveAngleRoll = 0x4000 / 3;
	}

	s_3Dmove();
	if(sp1E == 0)
		objsound(SOUND_GENERAL_MOVING_WATER);
}

void func_802F5FD8(void)
{
	struct Object* sp1C = o->parentObj;
	struct Object* sp18;

	if(o->oTimer == 300 * FRAME_RATE_SCALER_INV)
		o->oTimer = 0;

	if(o->oTimer == 0 || o->oTimer == 50 * FRAME_RATE_SCALER_INV || o->oTimer == 150 * FRAME_RATE_SCALER_INV || o->oTimer == 200 * FRAME_RATE_SCALER_INV || o->oTimer == 250 * FRAME_RATE_SCALER_INV)
	{
		sp18		      = s_makeobj_nowpos(o, MODEL_WATER_RING, sm64::bhv::bhvMantaRayWaterRing());
		sp18->oFaceAngleYaw   = o->oMoveAngleYaw;
		sp18->oFaceAnglePitch = o->oMoveAnglePitch + 0x4000;
		sp18->oPosX	      = o->oPosX + 200.0f * sins(o->oMoveAngleYaw + 0x8000);
		sp18->oPosY	      = o->oPosY + 10.0f + 200.0f * sins(o->oMoveAnglePitch);
		sp18->oPosZ	      = o->oPosZ + 200.0f * coss(o->oMoveAngleYaw + 0x8000);
		sp18->oWaterRingIndex = sp1C->oWaterRingMgrNextRingIndex;

		sp1C->oWaterRingMgrNextRingIndex++;
		if(sp1C->oWaterRingMgrNextRingIndex > 0x2710)
			sp1C->oWaterRingMgrNextRingIndex = 0;
	}
}

void bhv_manta_ray_loop(void)
{
	switch(o->oAction)
	{
		case 0:
			func_802F5E20();
			func_802F5FD8();
			if(o->oMantaUnk1AC == 5)
			{
				s_kemuri();
				s_enemyset_star(-3180.0f, -3600.0f, 120.0f);
				o->oAction = 1;
			}
			break;

		case 1:
			func_802F5E20();
			break;
	}

	if(o->oInteractStatus & INT_STATUS_INTERACTED)
		o->oInteractStatus = 0;
}
