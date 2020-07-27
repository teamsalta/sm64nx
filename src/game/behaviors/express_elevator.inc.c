// express_elevator.c.inc

void bhv_wdw_express_elevator_loop(void)
{
	o->oVelY = 0.0f;
	;

	if(o->oAction == 0)
	{
		if(s_rideon_player())
			o->oAction++;
	}
	else if(o->oAction == 1)
	{
		o->oVelY = -20.0f;
		o->oPosY += o->oVelY * FRAME_RATE_SCALER;
		objsound_level(SOUND_ENV_ELEVATOR4);
		if(o->oTimer > 132 * FRAME_RATE_SCALER_INV)
			o->oAction++;
	}
	else if(o->oAction == 2)
	{
		if(o->oTimer > 110 * FRAME_RATE_SCALER_INV)
			o->oAction++;
	}
	else if(o->oAction == 3)
	{
		o->oVelY = 10.0f;
		o->oPosY += o->oVelY * FRAME_RATE_SCALER;
		objsound_level(SOUND_ENV_ELEVATOR4);
		if(o->oPosY >= o->oHomeY)
		{
			o->oPosY = o->oHomeY;
			o->oAction++;
		}
	}
	else if(!s_rideon_player())
		o->oAction = 0;
}
