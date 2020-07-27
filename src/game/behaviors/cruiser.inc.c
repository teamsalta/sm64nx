// cruiser.c.inc

void bhv_rr_cruiser_wing_init(void)
{
	o->oRRCruiserWingUnkF4 = o->oFaceAngleYaw;
	o->oRRCruiserWingUnkF8 = o->oFaceAnglePitch;
}

void bhv_rr_cruiser_wing_loop(void)
{
	if(o->oBehParams2ndByte == 0)
	{
		o->oFaceAngleYaw   = o->oRRCruiserWingUnkF4 + sins(o->oTimer * 0x400 / FRAME_RATE_SCALER_INV) * 8192.0f;
		o->oFaceAnglePitch = o->oRRCruiserWingUnkF8 + coss(o->oTimer * 0x400 / FRAME_RATE_SCALER_INV) * 2048.0f;
	}
	else
	{
		o->oFaceAngleYaw   = o->oRRCruiserWingUnkF4 - sins(o->oTimer * 0x400 / FRAME_RATE_SCALER_INV) * 8192.0f;
		o->oFaceAnglePitch = o->oRRCruiserWingUnkF8 + coss(o->oTimer * 0x400 / FRAME_RATE_SCALER_INV) * 2048.0f;
	}
	if(o->oTimer == 64 * FRAME_RATE_SCALER_INV)
	{
		objsound(SOUND_GENERAL_BOAT_ROCK);
		o->oTimer = 0;
	}
}
