
void bhv_swing_platform_init(void)
{
	o->oSwingPlatformAngle = 0x2000;
}

void bhv_swing_platform_update(void)
{
	s32 startRoll = o->oFaceAngleRoll;

	if(o->oFaceAngleRoll < 0)
	{
		o->oSwingPlatformSpeed += 4.0f * FRAME_RATE_SCALER;
	}
	else
	{
		o->oSwingPlatformSpeed -= 4.0f * FRAME_RATE_SCALER;
	}

	o->oSwingPlatformAngle += o->oSwingPlatformSpeed * FRAME_RATE_SCALER;
	o->oFaceAngleRoll = o->oSwingPlatformAngle;
	o->oAngleVelRoll  = o->oFaceAngleRoll - startRoll;
}
