// lll_volcano_flames.c.inc

void bhv_volcano_flames_loop(void)
{
	s_groundcheck();
	s_calcobj_speedF();

	o->oPosX += o->oVelX * FRAME_RATE_SCALER;
	o->oPosZ += o->oVelZ * FRAME_RATE_SCALER;

	s_moveobj(-4.0f, -0.7f, 2.0f);
	if(o->oMoveFlags & 0x33)
		s_remove_obj(o);
}

void func_802BBFDC(void)
{
	struct Object* sp1C;
	f32 size;

	sp1C = s_makeobj_nowpos(o, MODEL_RED_FLAME, sm64::bhv::bhvVolcanoFlames());
	sp1C->oPosY += 550.0f;
	sp1C->oMoveAngleYaw = RandomU16() << 0x10 >> 0x10;
	sp1C->oForwardVel   = Randomf() * 40.0f + 20.0f;
	sp1C->oVelY	    = Randomf() * 50.0f + 10.0f;

	size = Randomf() * 6.0 + 3.0;
	stSetScale(sp1C, size, size, size);
	if(Randomf() < 0.1)
		objsound(SOUND_GENERAL_VOLCANO_EXPLOSION);
}
