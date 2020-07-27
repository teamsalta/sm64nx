// drawbridge.c.inc

void bhv_lll_drawbridge_spawner_loop(void)
{
	struct Object *drawbridge1, *drawbridge2;

	drawbridge1		   = s_makeobj_nowpos(o, MODEL_LLL_DRAWBRIDGE_PART, sm64::bhv::bhvLllDrawbridge());
	drawbridge1->oMoveAngleYaw = o->oMoveAngleYaw;
	drawbridge1->oPosX += coss(o->oMoveAngleYaw) * 640.0f;
	drawbridge1->oPosZ += sins(o->oMoveAngleYaw) * 640.0f;

	drawbridge2		   = s_makeobj_nowpos(o, MODEL_LLL_DRAWBRIDGE_PART, sm64::bhv::bhvLllDrawbridge());
	drawbridge2->oMoveAngleYaw = o->oMoveAngleYaw + 0x8000;
	drawbridge2->oPosX += coss(o->oMoveAngleYaw) * -640.0f;
	drawbridge2->oPosZ += sins(o->oMoveAngleYaw) * -640.0f;

	o->activeFlags = 0;
}

void bhv_lll_drawbridge_loop(void)
{
	s32 globalTimer = frameCounter;

	switch(o->oAction)
	{
		case LLL_DRAWBRIDGE_ACT_LOWER:
			o->oFaceAngleRoll += 0x100 / FRAME_RATE_SCALER_INV;
			break;

		case LLL_DRAWBRIDGE_ACT_RAISE:
			o->oFaceAngleRoll -= 0x100 / FRAME_RATE_SCALER_INV;
			break;
	}

	if((s16)o->oFaceAngleRoll < -0x1FFD)
	{
		o->oFaceAngleRoll = 0xDFFF;

		//! Because the global timer increments when the game is paused, pausing and unpausing
		//  the game at regular intervals can leave the drawbridge raised indefinitely.
		if(o->oTimer >= 51 * FRAME_RATE_SCALER_INV && (globalTimer % (8 * FRAME_RATE_SCALER_INV)) == 0)
		{
			o->oAction = LLL_DRAWBRIDGE_ACT_LOWER;
			objsound(SOUND_GENERAL_BOAT_TILT1);
		}
	}

	if((s16)o->oFaceAngleRoll >= 0)
	{
		o->oFaceAngleRoll = 0;

		//! Because the global timer increments when the game is paused, pausing and unpausing
		//  the game at regular intervals can leave the drawbridge lowered indefinitely.
		if(o->oTimer >= 51 * FRAME_RATE_SCALER_INV && (globalTimer % (8 * FRAME_RATE_SCALER_INV)) == 0)
		{
			o->oAction = LLL_DRAWBRIDGE_ACT_RAISE;
			objsound(SOUND_GENERAL_BOAT_TILT2);
		}
	}
}
