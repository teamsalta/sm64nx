// orange_number.c.inc

void s_numbers_init(void)
{
	o->oAnimState = o->oBehParams2ndByte;
	o->oVelY      = 26.0f;
}

void s_numbers_main(void)
{
	struct Object* sp1C;

	o->oPosY += o->oVelY * FRAME_RATE_SCALER;
	o->oVelY -= 2.0f * FRAME_RATE_SCALER;

	if(o->oVelY < -21.0f)
		o->oVelY = 14.0;

	if(o->oTimer == 35 * FRAME_RATE_SCALER_INV)
	{
		sp1C = s_makeobj_nowpos(o, MODEL_SPARKLES, sm64::bhv::bhvGoldenCoinSparkles());
		sp1C->oPosY -= 30.f;
		o->activeFlags = 0;
	}
}
