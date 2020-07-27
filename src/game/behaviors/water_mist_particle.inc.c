// water_mist_particle.c.inc
// TODO: Is this really "mist"?

void bhv_water_mist_spawn_loop(void)
{
	func_802ADBBC(0x20000);
	s_makeobj_nowpos(o, MODEL_MIST, sm64::bhv::bhvWaterMist());
}

void bhv_water_mist_loop(void)
{
	f32 sp1C;
	if(o->oTimer == 0)
	{
		o->oMoveAngleYaw = gMarioObject->oMoveAngleYaw;
		translate_object_xz_random(o, 10.0f);
	}
	s_optionmove_F();
	o->oOpacity -= 42;
	sp1C = (254 - o->oOpacity) / 254.0 * 1.0 + 0.5; // seen this before
	s_set_scale(sp1C);
	if(o->oOpacity < 2)
		s_remove_obj(o);
}
