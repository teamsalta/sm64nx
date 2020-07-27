// tox_box.c.inc

s8 D_8032F8F0[]	 = {4, 1, 4, 1, 6, 1, 6, 1, 5, 1, 5, 1, 6, 1, 6, 1, 5, 1, 2, 4, 1, 4, 1, 4, 1, 2, 5, 1, 5, 1, 7, 1, 7, 1, 4, 1, 4, 1, 7, 1, 7, 1, 5, 1, 5, 1, 5, 1, 2, 4, 1, -1};
s8 D_8032F924[]	 = {4, 1, 4, 1, 7, 1, 7, 1, 7, 1, 2, 6, 1, 6, 1, 6, 1, 5, 1, 5, 1, 6, 1, 5, 1, 5, 1, 2, 4, 1, 4, 1, 7, 1, -1};
s8 D_8032F948[]	 = {4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 2, 5, 1, 5, 1, 5, 1, 5, 1, 5, 1, 7, 1, 2, 6, 1, 6, 1, 5, 1, 2, 4, 1, 7, 1, -1};
s8* D_8032F96C[] = {D_8032F8F0, D_8032F924, D_8032F948};

void func_802BCF78(void)
{
	if(o->oDistanceToMario < 3000.0f)
	{
		s_call_Viewshake(SHAKE_POS_SMALL);
	}
}

void func_802BCFC4(f32 speed, f32 a1, s16 pitch, s16 roll) // 0x18 0x1c 0x22 0x26
{
	o->oPosY       = 99.41124 * sins((f32)((o->oTimer / FRAME_RATE_SCALER_INV) + 1) / 8 * 0x8000) + o->oHomeY + 3.0f;
	o->oForwardVel = speed;
	o->oUnkC0      = a1;

	if(pitch)
	{
		u8 pitchFace	   = o->oFaceAnglePitch / pitch;
		o->oFaceAnglePitch = (pitchFace * pitch) + pitch * o->oTimer / (7 * FRAME_RATE_SCALER_INV);
	}

	if((s16)o->oFaceAnglePitch < 0)
	{
		roll = -roll;
	}

	if(roll)
	{
		u8 rollFace	  = o->oFaceAngleRoll / roll;
		o->oFaceAngleRoll = (rollFace * roll) + roll * o->oTimer / (7 * FRAME_RATE_SCALER_INV);
	}

	func_802A2A38();
	if(o->oTimer == 7 * FRAME_RATE_SCALER_INV)
	{
		o->oAction = func_802A32E0();
		objsound(SOUND_GENERAL_UNK46);
	}
}

void ActionToxBox4(void)
{
	func_802BCFC4(64.0f, 0.0f, DEGREES(90), 0);
}
void ActionToxBox5(void)
{
	func_802BCFC4(-64.0f, 0.0f, DEGREES(-90), 0);
}
void ActionToxBox6(void)
{
	func_802BCFC4(0.0f, -64.0f, 0, DEGREES(90));
}
void ActionToxBox7(void)
{
	func_802BCFC4(0.0f, 64.0f, 0, DEGREES(-90));
}

void ActionToxBox1(void)
{
	o->oForwardVel = 0.0f;

	if(o->oTimer == 0)
	{
		func_802BCF78();
	}

	o->oPosY = o->oHomeY + 3.0f;

	if(o->oTimer == 20 * FRAME_RATE_SCALER_INV)
	{
		o->oAction = func_802A32E0();
	}
}

void ActionToxBox2(void)
{
	if(o->oTimer == 20 * FRAME_RATE_SCALER_INV)
		o->oAction = func_802A32E0();
}

void ActionToxBox3(void)
{
	if(o->oTimer == 20 * FRAME_RATE_SCALER_INV)
		o->oAction = func_802A32E0();
}

void ActionToxBox0(void)
{
	s8* sp1C   = D_8032F96C[o->oBehParams2ndByte];
	o->oAction = func_802A32A4(sp1C);
}

void (*sToxBoxActions[])(void) = {ActionToxBox0, ActionToxBox1, ActionToxBox2, ActionToxBox3, ActionToxBox4, ActionToxBox5, ActionToxBox6, ActionToxBox7};

void bhv_tox_box_loop(void)
{
	s_modejmp(sToxBoxActions);
	stMainMoveBG();
}
