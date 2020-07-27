// lll_octagonal_rotating_mesh.c.inc

s16 D_8032F860[] = {2, 30, 0x4000, 0, 1, 220, 900, 30, 1, 30, 0, -30, 2, 30, -0x4000, 0, 1, 220, 900, 30, 1, 30, 0, -30, 3, 0};

s16 D_8032F894[] = {4, 0, 0, 0, 1, 475, 900, 30, 1, 30, 0, -30, 2, 30, 0x8000, 0, 1, 475, 900, 30, 1, 30, 0, -30, 3, 0};

s16* D_8032F8C8[] = {D_8032F860, D_8032F894};

s32 func_802BB414(s16* a0, s32 a1)
{
	switch(a0[a1])
	{
		case 4:
			o->oMoveAngleYaw = a0[a1 + 2];
			o->oForwardVel	 = a0[a1 + 3] / 100.0f;

			if(s_rideon_player())
			{
				a1 += 4;
				o->oTimer = 0;
			}
			break;
		case 2:
			o->oMoveAngleYaw = a0[a1 + 2];
			o->oForwardVel	 = a0[a1 + 3] / 100.0f;

			if(o->oTimer > a0[a1 + 1] * FRAME_RATE_SCALER_INV)
			{
				a1 += 4;
				o->oTimer = 0;
			}
			break;
		case 1:
			approach_f32_signed(&o->oForwardVel, a0[a1 + 2] / 100.0f, (a0[a1 + 3] / 100.0f) * FRAME_RATE_SCALER);

			if(o->oTimer > a0[a1 + 1] * FRAME_RATE_SCALER_INV)
			{
				a1 += 4;
				o->oTimer = 0;
			}
			break;
		case 3:
			o->oForwardVel = 0.0f;
			a1	       = 0;
			break;
	}
	return a1;
}

s32 func_802BB680(s32* a0, f32* a1, s32 a2, s32 a3)
{
	if(s_rideon_player())
	{
		if(a0[0] < 0x4000)
			a0[0] += a2;
		else
			a0[0] = 0x4000;
	}
	else
	{
		if(a0[0] > 0)
			a0[0] -= a2;
		else
			a0[0] = 0;
	}
	a1[0] = sins(a0[0]) * a3;
	if(a0[0] == 0 || a0[0] == 0x4000)
		return 1;
	else
		return 0;
}

void bhv_lll_moving_octagonal_mesh_platform_loop(void)
{
	if(o->oAction == 0)
	{
		o->oHorizontalMovementUnkF8 = 0;
		o->oAction++;
	}
	else
	{
		o->oHorizontalMovementUnkF8 = func_802BB414(D_8032F8C8[o->oBehParams2ndByte], o->oHorizontalMovementUnkF8);
	}

	s_optionmove_F();

	if(func_802BB680(&o->oHorizontalMovementUnk104, &o->oHorizontalMovementUnk108, 0x400 / FRAME_RATE_SCALER_INV, -80))
	{
		o->oHorizontalMovementUnkF4 += 0x800 / FRAME_RATE_SCALER_INV;
		o->oHorizontalMovementUnk100 -= sins(o->oHorizontalMovementUnkF4) * 2 / FRAME_RATE_SCALER_INV;
	}
	o->oPosY = o->oHorizontalMovementUnk100 + o->oHomeY + o->oHorizontalMovementUnk108;
}
