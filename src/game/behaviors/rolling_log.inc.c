// rolling_log.c.inc

// why are the falling platforms and rolling logs grouped
// together? seems strange, but it also cooresponds to the
// behavior script grouping if the same file is assumed.
// hypothesis is that the object in the middle here used to be
// a rolling log of another variation.

void s_don_maruta_init(void)
{
	o->oPitouneUnkF8   = 3970.0f;
	o->oPitouneUnkFC   = 3654.0f;
	o->oPitouneUnkF4   = 271037.0f;
	o->oMoveAngleYaw   = 8810;
	o->oForwardVel	   = 0;
	o->oVelX	   = 0;
	o->oVelZ	   = 0;
	o->oFaceAnglePitch = 0;
	o->oAngleVelPitch  = 0;
}

void don_maruta_PlayerOnCheck(void)
{
	f32 sp24;

	if(gMarioObject->platform == o)
	{
		sp24 = (gMarioObject->header.gfx.pos[2] - o->oPosZ) * coss(-o->oMoveAngleYaw) - (gMarioObject->header.gfx.pos[0] - o->oPosX) * sins(-o->oMoveAngleYaw);
		if(sp24 > 0)
			o->oAngleVelPitch += 0x10 / FRAME_RATE_SCALER_INV;
		else
			o->oAngleVelPitch -= 0x10 / FRAME_RATE_SCALER_INV;

		if(o->oAngleVelPitch > 0x200)
			o->oAngleVelPitch = 0x200;

		if(o->oAngleVelPitch < -0x200)
			o->oAngleVelPitch = -0x200;
	}
	else
	{
		if(ObjApproach(o, o->oHomeX, o->oHomeY, o->oHomeZ, 100))
		{
			if(o->oAngleVelPitch != 0)
			{
				if(o->oAngleVelPitch > 0)
					o->oAngleVelPitch -= 0x10 / FRAME_RATE_SCALER_INV;
				else
					o->oAngleVelPitch += 0x10 / FRAME_RATE_SCALER_INV;

				if(o->oAngleVelPitch < 0x10 && o->oAngleVelPitch > -0x10)
					o->oAngleVelPitch = 0;
			}
		}
		else
		{
			if(o->oAngleVelPitch != 0x100)
			{
				if(o->oAngleVelPitch > 0x100)
					o->oAngleVelPitch -= 0x10;
				else
					o->oAngleVelPitch += 0x10 / FRAME_RATE_SCALER_INV;

				if(o->oAngleVelPitch < 0x110 && o->oAngleVelPitch > 0xF0)
					o->oAngleVelPitch = 0x100;
			}
		}
	}
}

void s_don_maruta_main(void)
{
	f32 prevX = o->oPosX;
	f32 prevZ = o->oPosZ;

	don_maruta_PlayerOnCheck();

	o->oForwardVel = o->oAngleVelPitch / 0x40;
	o->oVelX       = o->oForwardVel * sins(o->oMoveAngleYaw);
	o->oVelZ       = o->oForwardVel * coss(o->oMoveAngleYaw);

	o->oPosX += o->oVelX * FRAME_RATE_SCALER;
	o->oPosZ += o->oVelZ * FRAME_RATE_SCALER;

	if(o->oPitouneUnkF4 < sqr(o->oPosX - o->oPitouneUnkF8) + sqr(o->oPosZ - o->oPitouneUnkFC))
	{
		o->oForwardVel = 0;
		o->oPosX       = prevX;
		o->oPosZ       = prevZ;
		o->oVelX       = 0;
		o->oVelZ       = 0;
	}

	o->oFaceAnglePitch += o->oAngleVelPitch * FRAME_RATE_SCALER;

	if(absf_2(o->oFaceAnglePitch & 0x1FFF) < 528.0f && o->oAngleVelPitch != 0)
	{
		objsound(SOUND_GENERAL_UNKNOWN1_2);
	}
}

void battan_Move(void)
{
	o->oRollingLogUnkF4 += 4.0f * FRAME_RATE_SCALER;
	o->oAngleVelPitch += o->oRollingLogUnkF4 * FRAME_RATE_SCALER;
	o->oFaceAnglePitch -= o->oAngleVelPitch * FRAME_RATE_SCALER;

	if(o->oFaceAnglePitch < -0x4000)
	{
		o->oFaceAnglePitch  = -0x4000;
		o->oAngleVelPitch   = 0;
		o->oRollingLogUnkF4 = 0;
		o->oAction	    = 2;
		objsound(SOUND_GENERAL_BIG_POUND);
		Viewshaking(SHAKE_POS_LARGE, o->oPosX, o->oPosY, o->oPosZ);
	}
}

void battan_Home(void)
{
	o->oAngleVelPitch = 0x90;
	o->oFaceAnglePitch += o->oAngleVelPitch * FRAME_RATE_SCALER;
	if(o->oFaceAnglePitch > 0)
		o->oFaceAnglePitch = 0;

	if(o->oTimer == 200 * FRAME_RATE_SCALER_INV)
		o->oAction = 0;
}

void s_fm_battan_main(void)
{
	switch(o->oAction)
	{
		case 0:
			if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 1000))
			{
				o->oAction = 1;
				objsound(SOUND_GENERAL_QUIET_POUND2);
			}
			break;

		case 1:
			battan_Move();
			break;

		case 2:
			if(o->oTimer < 8 * FRAME_RATE_SCALER_INV)
			{
				o->oPosY = o->oHomeY + sins(o->oTimer * 0x1000 / FRAME_RATE_SCALER_INV) * 10.0f;
			}
			if(o->oTimer == 50 * FRAME_RATE_SCALER_INV)
			{
				objsound(SOUND_GENERAL_UNK45);
				o->oAction = 3;
			}
			break;

		case 3:
			battan_Home();
			break;
	}
}

void s_fm_maruta_init(void)
{
	o->oPitouneUnkF8 = 5120.0f;
	o->oPitouneUnkFC = 6016.0f;
	o->oPitouneUnkF4 = 1048576.0f;

	o->oMoveAngleYaw   = 0x3FFF;
	o->oForwardVel	   = 0;
	o->oVelX	   = 0;
	o->oVelZ	   = 0;
	o->oFaceAnglePitch = 0;
	o->oAngleVelPitch  = 0;
}
