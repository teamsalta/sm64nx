// wind_particle.c.inc

struct ObjectHitbox sWindParticleHitbox = {
    /* interactType: */ INTERACT_WIND,
    /* downOffset: */ 0,
    /* damageOrCoinValue: */ 0,
    /* health: */ 0,
    /* numLootCoins: */ 0,
    /* radius: */ 20,
    /* height: */ 70,
    /* hurtboxRadius: */ 20,
    /* hurtboxHeight: */ 70,
};

void bhv_white_wind_particle_loop(void)
{
	struct Object* sp34;
	f32 sp30;
	f32 sp2C;
	f32 sp28;
	s_set_hitparam(o, &sWindParticleHitbox);
	if(o->oTimer == 0)
	{
		o->oWhiteWindParticleUnkF4 = s_find_obj(sm64::bhv::bhvWalkingPenguin());
		translate_object_xyz_random(o, 100.0f);
		o->oForwardVel = coss(o->oMoveAnglePitch) * 100.0f;
		o->oVelY       = sins(o->oMoveAnglePitch) * -100.0f;
		o->oMoveAngleYaw += random_f32_around_zero(o->oBehParams2ndByte * 500 / FRAME_RATE_SCALER_INV);
		o->oOpacity = 100;
	}
	s_optionmove_F();
	if(o->oTimer > 15 * FRAME_RATE_SCALER_INV)
		s_remove_obj(o);
	sp34 = o->oWhiteWindParticleUnkF4;
	if(sp34 != 0)
	{
		sp2C = sp34->oWalkingPenguinUnk100 - o->oPosX;
		sp28 = sp34->oWalkingPenguinUnk104 - o->oPosZ;
		sp30 = sqrtf(sp2C * sp2C + sp28 * sp28);
		if(sp30 < 300.0f)
		{
			s_remove_obj(o);
			s_hitOFF();
		}
	}
}

void func_802C76E0(s32 a0, f32 a1, f32 a2, f32 a3, f32 a4)
{
	if((frameCounter & 1) != 0)
	{
		spawn_object_relative_with_scale(a0, a2, a3, a4, 0.5f, o, MODEL_WHITE_PARTICLE_DL, sm64::bhv::bhvTinyWhiteWindParticle());
		spawn_object_relative_with_scale(a0, a2, a3, a4, a1, o, MODEL_NONE, sm64::bhv::bhvWindParticle());
	}
	else
	{
		spawn_object_relative_with_scale(a0, a2, a3, a4, a1, o, MODEL_MIST, sm64::bhv::bhvWindParticle());
	}

	spawn_object_relative_with_scale(a0, a2, a3, a4, a1, o, MODEL_NONE, sm64::bhv::bhvWindParticle());
}
