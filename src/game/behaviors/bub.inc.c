// bub.c.inc

// NOTE: These first set of functions spawn a school of bub depending on objF4's
// value. The later action functions seem to check Y distance to Mario and proceed
// to do nothing, which indicates this behavior set is incomplete.

// TODO: Rename these. These have nothing to do with birds.
void ActionBirdChirpChirp0(void)
{
	s32 i;
	s32 sp18 = o->oBirdChirpChirpUnkF4;

	if(o->oDistanceToMario < 1500.0f && !sm64::config().camera().disableDistanceClip())
	{
		for(i = 0; i < sp18; i++)
		{
			s_makeobj_nowpos(o, MODEL_BUB, sm64::bhv::bhvBub());
		}

		o->oAction = 1;
	}
}

void ActionBirdChirpChirp1(void)
{
	if(gMarioObject->oPosY - o->oPosY > 2000.0f)
		o->oAction = 2;
}

void ActionBirdChirpChirp2(void)
{
	o->oAction = 3;
}

void ActionBirdChirpChirp3(void)
{
	o->oAction = 0;
}

void (*sBirdChirpChirpActions[])(void) = {ActionBirdChirpChirp0, ActionBirdChirpChirp1, ActionBirdChirpChirp2, ActionBirdChirpChirp3};

void bhv_bird_chirp_chirp_loop(void)
{
	s_modejmp(sBirdChirpChirpActions);
}

void func_802C0240(s32 a0)
{
	f32 sp1C = o->parentObj->oPosY;
	if(sp1C - 100.0f - o->oCheepCheepUnk104 < o->oPosY && o->oPosY < sp1C + 1000.0f + o->oCheepCheepUnk104)
		o->oPosY = approach_f32_symmetric(o->oPosY, o->oCheepCheepUnkF8, a0 * FRAME_RATE_SCALER);
	else
	{
	}
}

void ActionCheepCheep0(void)
{
	o->oCheepCheepUnkFC  = Randomf() * 100.0f;
	o->oCheepCheepUnk104 = Randomf() * 300.0f;
	o->oAction	     = 1;
}

void ActionCheepCheep1(void)
{
	f32 dy;
	if(o->oTimer == 0)
	{
		o->oForwardVel	     = Randomf() * 2 + 2;
		o->oCheepCheepUnk108 = Randomf();
	}
	dy = o->oPosY - gMarioObject->oPosY;
	if(o->oPosY < o->oCheepCheepUnkF4 - 50.0f)
	{
		if(dy < 0.0f)
			dy = 0.0f - dy;
		if(dy < 500.0f)
			func_802C0240(1);
		else
			func_802C0240(4);
	}
	else
	{
		o->oPosY = o->oCheepCheepUnkF4 - 50.0f;
		if(dy > 300.0f)
			o->oPosY = o->oPosY - 1.0f;
	}
	if(800.0f < s_calc_playerscope())
		o->oAngleToMario = s_calc_returnangle();
	s_chase_angleY(o->oAngleToMario, 0x100 / FRAME_RATE_SCALER_INV);
	if(o->oDistanceToMario < 200.0f)
		if(o->oCheepCheepUnk108 < 0.5)
			o->oAction = 2;
	if(o->oInteractStatus & INT_STATUS_INTERACTED)
		o->oAction = 2;
}

void ActionCheepCheep2(void)
{
	f32 dy;
	if(o->oTimer < 20 * FRAME_RATE_SCALER_INV)
	{
		if(o->oInteractStatus & INT_STATUS_INTERACTED)
			s_makeobj_nowpos(o, MODEL_WHITE_PARTICLE_SMALL, sm64::bhv::bhvSmallParticleSnow());
	}
	else
		o->oInteractStatus = 0;
	if(o->oTimer == 0)
		objsound(SOUND_GENERAL_MOVING_WATER);
	if(o->oForwardVel == 0.0f)
		o->oForwardVel = 6.0f;
	dy = o->oPosY - gMarioObject->oPosY;
	if(o->oPosY < o->oCheepCheepUnkF4 - 50.0f)
	{
		if(dy < 0.0f)
			dy = 0.0f - dy;
		if(dy < 500.0f)
			func_802C0240(2);
		else
			func_802C0240(4);
	}
	else
	{
		o->oPosY = o->oCheepCheepUnkF4 - 50.0f;
		if(dy > 300.0f)
			o->oPosY -= 1.0f;
	}
	if(s_calc_playerscope() > 800.0f)
		o->oAngleToMario = s_calc_returnangle();
	s_chase_angleY(o->oAngleToMario + 0x8000, 0x400 / FRAME_RATE_SCALER_INV);
	if(o->oTimer > 200 * FRAME_RATE_SCALER_INV && o->oDistanceToMario > 600.0f)
		o->oAction = 1;
}

void (*sCheepCheepActions[])(void) = {ActionCheepCheep0, ActionCheepCheep1, ActionCheepCheep2};

void bhv_cheep_cheep_loop(void)
{
	o->oCheepCheepUnkF4  = find_water_level(o->oPosX, o->oPosZ);
	o->oCheepCheepUnkF8  = gMarioObject->oPosY + o->oCheepCheepUnkFC;
	o->oWallHitboxRadius = 30.0f;
	s_enemybgcheck();
	s_modejmp(sCheepCheepActions);
	s_optionmove_F();
	if(o->parentObj->oAction == 2)
		s_remove_obj(o);
}
