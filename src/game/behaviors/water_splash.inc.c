// water_splash.c.inc

struct WaterSplashParams D_8032FDAC = {2, MODEL_WHITE_PARTICLE_SMALL, sm64::bhv::bhvWaterDrops(), 0, 0, 5.0f, 3.0f, 30.0f, 20.0f, 0.5f, 1.0f};
struct WaterSplashParams D_8032FDD0 = {34, MODEL_WHITE_PARTICLE_SMALL, sm64::bhv::bhvWaterDrops(), 0, 0, 2.0f, 3.0f, 20.0f, 20.0f, 0.5f, 1.0f};
struct WaterSplashParams D_8032FDF4 = {34, MODEL_FISH, sm64::bhv::bhvWaterDrops(), 0, 0, 2.0f, 3.0f, 20.0f, 20.0f, 1.0f, 0.0f};
struct WaterSplashParams D_8032FE18 = {98, MODEL_WHITE_PARTICLE_SMALL, sm64::bhv::bhvWaterDrops(), 0x6000, 0, 2.0f, 8.0f, 10.0f, 10.0f, 0.5f, 1.0f};

void bhv_water_splash_loop(void)
{
	s32 i;
	if(o->oTimer == 0)
		o->oPosY = mcWaterCheck(o->oPosX, o->oPosZ);
	if(o->oPosY > -10000.0f)
		for(i = 0; i < 3; i++)
			s_makeobj_initdata(o, &D_8032FDAC);
}

void bhv_water_drops_loop(void)
{
	UNUSED s32 unused;
	f32 waterLevel = mcWaterCheck(o->oPosX, o->oPosZ);
	if(o->oTimer == 0)
	{
		if(s_check_shapename(MODEL_FISH))
			o->header.gfx.node.flags &= ~4;
		else
			o->header.gfx.node.flags |= 4;
		o->oFaceAngleYaw = RandomU16();
	}
	o->oVelY -= 4.0f * FRAME_RATE_SCALER;
	o->oPosY += o->oVelY * FRAME_RATE_SCALER;
	if(o->oVelY < 0.0f)
	{
		if(waterLevel > o->oPosY)
		{
			s_makeobj_effect(0, 1.0f, o, MODEL_SPOT_ON_GROUND, sm64::bhv::bhvWaterSurfaceWhiteWave());
			s_remove_obj(o);
		}
		else if(o->oTimer > 20 * FRAME_RATE_SCALER_INV)
			s_remove_obj(o);
	}
	if(waterLevel < -10000.0f)
		s_remove_obj(o);
}

void bhv_surface_waves_loop(void)
{
	s_copy_worldXYZ(o, gMarioObject);
	o->oPosY = playerWorks->waterLevel + 5;
	if(!(gMarioObject->oMarioParticleFlags & 0x80))
	{
		gMarioObject->oActiveParticleFlags &= 0xFF7F;
		o->activeFlags = 0;
	}
}

void bhv_water_surface_white_wave_init(void)
{
	s_set_scale(Randomf() + 1.5);
}

void bhv_object_bubble_ripples_init(void)
{
	f32 sp1C = mcWaterCheck(o->oPosX, o->oPosZ);
	stSetScale(o, 0.5f, 1.0f, 0.5f);
	o->oPosY = sp1C + 5.0f;
}

void bhv_surface_wave_shrinking_init(void)
{
	UNUSED struct Object* sp1C;
	if((RandomU16() & 0xFF) <= 0) // weird
	{
		sp1C = s_makeobj_initdata(o, &D_8032FDF4);
		s_set_skeletonobj(sp1C, (struct Animation**)blue_fish_seg3_anims_0301C2B0, 0);
	}
}

void bhv_wave_trail_loop(void)
{
	f32 sp1C = mcWaterCheck(o->oPosX, o->oPosZ);
	if(o->oTimer == 0)
		if(frameCounter & 1)
			s_remove_obj(o);
	o->oPosY = sp1C + 5.0f;
	if(o->oTimer == 0)
		o->oWaveTrailUnkF8 = o->header.gfx.scale[0];
	if(o->oAnimState > 3)
	{
		o->oWaveTrailUnkF8 = o->oWaveTrailUnkF8 - 0.1;
		if(o->oWaveTrailUnkF8 < 0.0f)
			o->oWaveTrailUnkF8 = 0.0f;
		o->header.gfx.scale[0] = o->oWaveTrailUnkF8;
		o->header.gfx.scale[2] = o->oWaveTrailUnkF8;
	}
}
