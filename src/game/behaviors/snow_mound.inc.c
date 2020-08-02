// snow_mound.c.inc

void s_iwanami_parts_main(void)
{
	switch(o->oAction)
	{
		case 0:
			o->oVelX = -40.0f;
			o->oPosX += o->oVelX * FRAME_RATE_SCALER;

			if(o->oTimer >= 118 * FRAME_RATE_SCALER_INV)
				o->oAction = 1;

			objsound_level(SOUND_ENV_SINK_QUICKSAND);
			break;

		case 1:
			o->oVelX = -5.0f;
			o->oPosX += o->oVelX * FRAME_RATE_SCALER;

			o->oVelY = -10.0f;
			o->oPosY += o->oVelY * FRAME_RATE_SCALER;

			o->oPosZ = o->oHomeZ - 2.0f;

			if(o->oTimer > 50 * FRAME_RATE_SCALER_INV)
				o->activeFlags = 0;
			break;
	}
}

void s_iwanami_main(void)
{
	struct Object* sp1C;

	if(!PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 6000) || o->oPosY + 1000.0f < gMarioObject->header.gfx.pos[1])
		return;

	if(o->oTimer == 64 * FRAME_RATE_SCALER_INV || o->oTimer == 128 * FRAME_RATE_SCALER_INV || o->oTimer == 192 * FRAME_RATE_SCALER_INV || o->oTimer == 224 * FRAME_RATE_SCALER_INV || o->oTimer == 256 * FRAME_RATE_SCALER_INV)
		sp1C = s_makeobj_nowpos(o, MODEL_SL_SNOW_TRIANGLE, sm64::bhv::bhvSlidingSnowMound());

	if(o->oTimer == 256 * FRAME_RATE_SCALER_INV)
	{
		sp1C->header.gfx.scale[0] = 2.0f;
		sp1C->header.gfx.scale[1] = 2.0f;
	}

	if(o->oTimer >= 256 * FRAME_RATE_SCALER_INV)
		o->oTimer = 0;
}
