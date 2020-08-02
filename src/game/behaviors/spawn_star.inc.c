// spawn_star.c.inc

static struct ObjectHitbox sCollectStarHitbox = {
    /* interactType:      */ INTERACT_STAR_OR_KEY,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void s_polystar_init(void)
{
	s8 sp1F;
	u8 sp1E;

	sp1F = (o->oBehParams >> 24) & 0xFF;
	sp1E = BuGetStarFlag(activePlayerNo - 1, activeCourseNo - 1);
	if(sp1E & (1 << sp1F))
	{
		o->header.gfx.sharedChild = stageShapes[MODEL_TRANSPARENT_STAR];
	}
	else
	{
		o->header.gfx.sharedChild = stageShapes[MODEL_STAR];
	}

	s_set_hitparam(o, &sCollectStarHitbox);
}

void s_polystar_main(void)
{
	o->oFaceAngleYaw += 0x800 / FRAME_RATE_SCALER_INV;

	if(o->oInteractStatus & INT_STATUS_INTERACTED)
	{
		RemoveShape(o);
		o->oInteractStatus = 0;
	}
}

void s_enemy_star_init(void)
{
	o->oMoveAngleYaw	 = atan2s(o->oHomeZ - o->oPosZ, o->oHomeX - o->oPosX);
	o->oStarSpawnDisFromHome = sqrtf(sqr(o->oHomeX - o->oPosX) + sqr(o->oHomeZ - o->oPosZ));
	o->oVelY		 = (o->oHomeY - o->oPosY) / 30.0f;
	o->oForwardVel		 = o->oStarSpawnDisFromHome / 30.0f;
	o->oStarSpawnY		 = o->oPosY;
	if(o->oBehParams2ndByte == 0 || activeCourseNo == COURSE_BBH)
		cutscene_object(CUTSCENE_STAR_SPAWN, o);
	else
		cutscene_object(CUTSCENE_RED_COIN_STAR_SPAWN, o);

	s_begin_enemydemo(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
	o->activeFlags |= 0x20;
	s_hitOFF();
}

void s_enemy_star_main(void)
{
	switch(o->oAction)
	{
		case 0:
			o->oFaceAngleYaw += 0x1000 / FRAME_RATE_SCALER_INV;

			if(o->oTimer > 20 * FRAME_RATE_SCALER_INV)
				o->oAction = 1;
			break;

		case 1:
			ObjSpeedOn(o);
			o->oStarSpawnY += o->oVelY * FRAME_RATE_SCALER;
			o->oPosY = o->oStarSpawnY + sins((o->oTimer * 0x8000 / FRAME_RATE_SCALER_INV) / 30) * 400.0f;
			o->oFaceAngleYaw += 0x1000 / FRAME_RATE_SCALER_INV;
			s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());
			objsound_level(SOUND_ENV_STAR);
			if(o->oTimer == 30 * FRAME_RATE_SCALER_INV)
			{
				o->oAction     = 2;
				o->oForwardVel = 0;
				Na_NewStarAppearBgm(TRUE);
			}
			break;

		case 2:
			if(o->oTimer < 20 * FRAME_RATE_SCALER_INV)
				o->oVelY = 20 - (o->oTimer / FRAME_RATE_SCALER_INV);
			else
				o->oVelY = -10.0f;

			s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());
			ObjSpeedOn(o);
			o->oFaceAngleYaw = o->oFaceAngleYaw - (o->oTimer * 0x10 / FRAME_RATE_SCALER_INV) + 0x1000;
			objsound_level(SOUND_ENV_STAR);

			if(o->oPosY < o->oHomeY)
			{
				objsound(SOUND_GENERAL_STAR_APPEARS);
				s_hitON();
				o->oPosY   = o->oHomeY;
				o->oAction = 3;
			}
			break;

		case 3:
			o->oFaceAngleYaw += 0x800 * FRAME_RATE_SCALER;
			if(o->oTimer == 20 * FRAME_RATE_SCALER_INV)
			{
				demoseqcode = TRUE;
				s_end_enemydemo(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
				o->activeFlags &= ~0x20;
			}

			if(o->oInteractStatus & INT_STATUS_INTERACTED)
			{
				RemoveShape(o);
				o->oInteractStatus = 0;
			}
			break;
	}
}

static Object* EnemysetStar(Object* sp30, f32 sp34, f32 sp38, f32 sp3C)
{
	sp30		      = s_makeobj_absolute(o, 0, MODEL_STAR, sm64::bhv::bhvStarSpawnCoordinates(), o->oPosX, o->oPosY, o->oPosZ, 0, 0, 0);
	sp30->oBehParams      = o->oBehParams;
	sp30->oHomeX	      = sp34;
	sp30->oHomeY	      = sp38;
	sp30->oHomeZ	      = sp3C;
	sp30->oFaceAnglePitch = 0;
	sp30->oFaceAngleRoll  = 0;
	return sp30;
}

void s_enemyset_star(f32 sp20, f32 sp24, f32 sp28)
{
	struct Object* sp1C	= nullptr;
	sp1C			= EnemysetStar(sp1C, sp20, sp24, sp28);
	sp1C->oBehParams2ndByte = 0;
}

void s_coinset_star(f32 sp20, f32 sp24, f32 sp28)
{
	Object* sp1C = 0;
	sp1C			= EnemysetStar(sp1C, sp20, sp24, sp28);
	sp1C->oBehParams2ndByte = 1;
}

void s_extraset_star(f32 sp20, f32 sp24, f32 sp28)
{
	Object* sp1C = nullptr;
	sp1C			= EnemysetStar(sp1C, sp20, sp24, sp28);
	sp1C->oBehParams2ndByte = 1;
	sp1C->oInteractionSubtype |= INT_SUBTYPE_NO_EXIT;
}

void s_getcoins_star_init(void)
{
	s16 sp36;
	struct Object* sp30;

	if(activeCourseNo != 3)
		s_makeobj_nowpos(o, MODEL_TRANSPARENT_STAR, sm64::bhv::bhvRedCoinStarMarker());

	sp36 = s_count_obj(sm64::bhv::bhvRedCoin());
	if(sp36 == 0)
	{
		sp30		 = s_makeobj_absolute(o, 0, MODEL_STAR, sm64::bhv::bhvStar(), o->oPosX, o->oPosY, o->oPosZ, 0, 0, 0);
		sp30->oBehParams = o->oBehParams;
		o->activeFlags	 = 0;
	}

	o->oHiddenStarTriggerCounter = 8 - sp36;
}

void s_getcoins_star_main(void)
{
	redcoin_num = o->oHiddenStarTriggerCounter;
	switch(o->oAction)
	{
		case 0:
			if(o->oHiddenStarTriggerCounter == 8)
				o->oAction = 1;
			break;

		case 1:
			if(o->oTimer > 2 * FRAME_RATE_SCALER_INV)
			{
				s_coinset_star(o->oPosX, o->oPosY, o->oPosZ);
				s_kemuri();
				o->activeFlags = 0;
			}
			break;
	}
}
