// bomp.c.inc

void bhv_small_bomp_init(void)
{
	o->oFaceAngleYaw -= 0x4000;
	o->oSmallBompInitX = o->oPosX;
	o->oTimer	   = Randomf() * 100.0f / FRAME_RATE_SCALER;
}

void bhv_small_bomp_loop(void)
{
	switch(o->oAction)
	{
		case BOMP_ACT_WAIT:
			if(o->oTimer >= 101 * FRAME_RATE_SCALER_INV)
			{
				o->oAction     = BOMP_ACT_POKE_OUT;
				o->oForwardVel = 30.0f;
			}
			break;

		case BOMP_ACT_POKE_OUT:
			if(o->oPosX > 3450.0f)
			{
				o->oPosX       = 3450.0f;
				o->oForwardVel = 0;
			}

			if(o->oTimer == 15.0 * FRAME_RATE_SCALER_INV)
			{
				o->oAction     = BOMP_ACT_EXTEND;
				o->oForwardVel = 40.0f;
				objsound(SOUND_OBJ_UNKNOWN2);
			}
			break;

		case BOMP_ACT_EXTEND:
			if(o->oPosX > 3830.0f)
			{
				o->oPosX       = 3830.0f;
				o->oForwardVel = 0;
			}

			if(o->oTimer == 60 * FRAME_RATE_SCALER_INV)
			{
				o->oAction     = BOMP_ACT_RETRACT;
				o->oForwardVel = 10.0f;
				o->oMoveAngleYaw -= 0x8000;
				objsound(SOUND_OBJ_UNKNOWN2);
			}
			break;

		case BOMP_ACT_RETRACT:
			if(o->oPosX < 3330.0f)
			{
				o->oPosX       = 3330.0f;
				o->oForwardVel = 0;
			}

			if(o->oTimer == 90 * FRAME_RATE_SCALER_INV)
			{
				o->oAction     = BOMP_ACT_POKE_OUT;
				o->oForwardVel = 25.0f;
				o->oMoveAngleYaw -= 0x8000;
			}
			break;
	}
}

void bhv_large_bomp_init(void)
{
	o->oMoveAngleYaw += 0x4000;
	o->oTimer = Randomf() * 100.0f;
}

void bhv_large_bomp_loop(void)
{
	switch(o->oAction)
	{
		case BOMP_ACT_WAIT:
			if(o->oTimer >= 101 * FRAME_RATE_SCALER_INV)
			{
				o->oAction     = BOMP_ACT_POKE_OUT;
				o->oForwardVel = 30.0f;
			}
			break;

		case BOMP_ACT_POKE_OUT:
			if(o->oPosX > 3450.0f)
			{
				o->oPosX       = 3450.0f;
				o->oForwardVel = 0;
			}

			if(o->oTimer == 15.0 * FRAME_RATE_SCALER_INV)
			{
				o->oAction     = BOMP_ACT_EXTEND;
				o->oForwardVel = 10.0f;
				objsound(SOUND_OBJ_UNKNOWN2);
			}
			break;

		case BOMP_ACT_EXTEND:
			if(o->oPosX > 3830.0f)
			{
				o->oPosX       = 3830.0f;
				o->oForwardVel = 0;
			}

			if(o->oTimer == 60 * FRAME_RATE_SCALER_INV)
			{
				o->oAction     = BOMP_ACT_RETRACT;
				o->oForwardVel = 10.0f;
				o->oMoveAngleYaw -= 0x8000;
				objsound(SOUND_OBJ_UNKNOWN2);
			}
			break;

		case BOMP_ACT_RETRACT:
			if(o->oPosX < 3330.0f)
			{
				o->oPosX       = 3330.0f;
				o->oForwardVel = 0;
			}

			if(o->oTimer == 90 * FRAME_RATE_SCALER_INV)
			{
				o->oAction     = BOMP_ACT_POKE_OUT;
				o->oForwardVel = 25.0f;
				o->oMoveAngleYaw -= 0x8000;
			}
			break;
	}
}
