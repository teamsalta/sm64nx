// ground_particles.c.inc

void bhv_pound_white_puffs_init(void)
{
	func_802ADBBC(0x8000);
	func_802ADA94();
}

void func_802ADA94(void)
{
	s_makeeffect_chiri(&D_8032F3CC);
}

void bhv_unused_0e40_init(void)
{
	func_802ADBBC(0x4000);
	s_makeeffect_chiri(&D_8032F3E0);
}

void Unknown802ADAF4(void)
{
	struct Object* smoke = s_makeobj_nowpos_scale(o, MODEL_SMOKE, sm64::bhv::bhvWhitePuffSmoke2(), 1.0f);
	smoke->oForwardVel   = D_8032F3F4[0];
	smoke->oVelY	     = D_8032F3F4[1];
	smoke->oGravity	     = D_8032F3F4[2];
	s_random_XYZ_offset(smoke, D_8032F3F4[3]);
}

// TODO Fix name
void func_802ADBBC(u32 flags)
{
	o->parentObj->oActiveParticleFlags &= flags ^ -1; // Clear the flags given (could just be ~flags)
}

void bhv_ground_snow_init(void)
{
	func_802ADBBC(1 << 16);
	s_makeeffect_chiri(&D_8032F3FC);
}
