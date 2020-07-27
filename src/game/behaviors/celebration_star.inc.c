// celebration_star.c.inc

void bhv_celebration_star_init(void)
{
	o->oHomeX			= gMarioObject->header.gfx.pos[0];
	o->oPosY			= gMarioObject->header.gfx.pos[1] + 30.0f;
	o->oHomeZ			= gMarioObject->header.gfx.pos[2];
	o->oMoveAngleYaw		= gMarioObject->header.gfx.angle[1] + 0x8000;
	o->oCelebStarDiameterOfRotation = 100;
#if BUGFIX_STAR_BOWSER_KEY
	if(activeStageNo == LEVEL_BOWSER_1 || activeStageNo == LEVEL_BOWSER_2)
	{
		o->header.gfx.sharedChild = stageShapes[MODEL_BOWSER_KEY];
		o->oFaceAnglePitch	  = 0;
		o->oFaceAngleRoll	  = 49152;
		s_set_scale(0.1f);
		o->oCelebStarUnkF4 = 1;
	}
	else
	{
		o->header.gfx.sharedChild = stageShapes[MODEL_STAR];
		o->oFaceAnglePitch	  = 0;
		o->oFaceAngleRoll	  = 0;
		s_set_scale(0.4f);
		o->oCelebStarUnkF4 = 0;
	}
#else
	o->header.gfx.sharedChild = stageShapes[MODEL_STAR];
	s_set_scale(0.4f);
	o->oFaceAnglePitch = 0;
	o->oFaceAngleRoll  = 0;
#endif
}

void CelebrationStarSpinAroundMarioLoop(void)
{
	o->oPosX = o->oHomeX + sins(o->oMoveAngleYaw) * (f32)(o->oCelebStarDiameterOfRotation / 2);
	o->oPosZ = o->oHomeZ + coss(o->oMoveAngleYaw) * (f32)(o->oCelebStarDiameterOfRotation / 2);
	o->oPosY += 5.0f * FRAME_RATE_SCALER;
	o->oFaceAngleYaw += 0x1000 / FRAME_RATE_SCALER_INV;
	o->oMoveAngleYaw += 0x2000 / FRAME_RATE_SCALER_INV;

	if(o->oTimer == 40 * FRAME_RATE_SCALER_INV)
		o->oAction = CELEB_STAR_ACT_FACE_CAMERA;
	if(o->oTimer < 35 * FRAME_RATE_SCALER_INV)
	{
		s_makeobj_nowpos(o, MODEL_SPARKLES, sm64::bhv::bhvCelebrationStarSparkle());
		o->oCelebStarDiameterOfRotation++;
	}
	else
		o->oCelebStarDiameterOfRotation -= 20 / FRAME_RATE_SCALER_INV;
}

void CelebrationStarFaceCameraLoop(void)
{
	if(o->oTimer < 10 * FRAME_RATE_SCALER_INV)
	{
#if BUGFIX_STAR_BOWSER_KEY
		if(o->oCelebStarUnkF4 == 0)
		{
			s_set_scale((f32)o->oTimer / 10.0 * FRAME_RATE_SCALER);
		}
		else
		{
			s_set_scale((f32)o->oTimer / 30.0 * FRAME_RATE_SCALER);
		}
#else
		 s_set_scale((f32)o->oTimer / 10.0 * FRAME_RATE_SCALER);
#endif
		o->oFaceAngleYaw += 0x1000 / FRAME_RATE_SCALER_INV;
	}
	else
	{
		o->oFaceAngleYaw = gMarioObject->header.gfx.angle[1];
	}

	if(o->oTimer == 59 * FRAME_RATE_SCALER_INV)
		o->activeFlags = 0;
}

void bhv_celebration_star_loop(void)
{
	switch(o->oAction)
	{
		case CELEB_STAR_ACT_SPIN_AROUND_MARIO:
			CelebrationStarSpinAroundMarioLoop();
			break;

		case CELEB_STAR_ACT_FACE_CAMERA:
			CelebrationStarFaceCameraLoop();
			break;
	}
}

void bhv_celebration_star_sparkle_loop(void)
{
	o->oPosY -= 15.0f * FRAME_RATE_SCALER;

	if(o->oTimer == 12 * FRAME_RATE_SCALER_INV)
		o->activeFlags = 0;
}

void bhv_star_key_collection_puff_spawner_loop(void)
{
	s_burneffect(0, 10, 30.0f);
	o->activeFlags = 0;
}
