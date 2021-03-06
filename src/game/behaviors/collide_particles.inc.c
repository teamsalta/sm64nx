// collide_particles.c.inc

u16 D_8032F2CC[] = {0xD000, 0, 0x3000, 0, 0xDE67, 0x2199, 0x2199, 0x2199, 0xDE67, 0xDE67, 0x2199, 0xDE67};

u16 D_8032F2E4[] = {0xE000, 0, 0, 0, 0x2000, 0, 0xE99A, 0x1666, 0x1666, 0x1666, 0xE99A, 0xE99A, 0x1666, 0xE99A};

void bhv_punch_tiny_triangle_loop(void)
{
	s16 sp1E;
	if(o->oTimer == 0)
	{
		sp1E			   = o->oMoveAngleYaw;
		o->oCollisionParticleUnkF4 = 1.28f;
		s_posoffset_mother(gMarioObject, 0.0f, 60.0f, 100.0f);
		o->oMoveAngleYaw = sp1E; // does s_posoffset_mother modify currentObject?
	}
	s_optionmove_F();
	o->oAnimState = 5;
	s_set_scale(o->oCollisionParticleUnkF4);
	o->oCollisionParticleUnkF4 -= 0.2f;
	if(gDebugInfo[4][0] + 6 < o->oTimer)
		s_remove_obj(o);
}

void bhv_punch_tiny_triangle_init(void)
{
	s32 i;
	UNUSED s32 unused;
	struct Object* triangle;
	for(i = 0; i < 6; i++)
	{
		triangle		= s_makeobj_nowpos(o, MODEL_DIRT_ANIMATION, sm64::bhv::bhvPunchTinyTriangle());
		triangle->oMoveAngleYaw = gMarioObject->oMoveAngleYaw + D_8032F2CC[2 * i] + 0x8000;
		triangle->oVelY		= sins(D_8032F2CC[2 * i + 1]) * 25.0f;
		triangle->oForwardVel	= coss(D_8032F2CC[2 * i + 1]) * 25.0f;
	}
}

void bhv_wall_tiny_star_particle_loop(void)
{
	s16 sp1E;
	if(o->oTimer == 0)
	{
		sp1E			   = o->oMoveAngleYaw;
		o->oCollisionParticleUnkF4 = 0.28f;
		s_posoffset_mother(gMarioObject, 0.0f, 30.0f, 110.0f);
		o->oMoveAngleYaw = sp1E;
	}
	s_optionmove_F();
	o->oAnimState = 4;
	s_set_scale(o->oCollisionParticleUnkF4);
	o->oCollisionParticleUnkF4 -= 0.015f;
}

void bhv_tiny_star_particles_init(void)
{
	s32 i;
	UNUSED s32 unused;
	struct Object* particle;
	for(i = 0; i < 7; i++)
	{
		particle		= s_makeobj_nowpos(o, MODEL_CARTOON_STAR, sm64::bhv::bhvWallTinyStarParticle());
		particle->oMoveAngleYaw = gMarioObject->oMoveAngleYaw + D_8032F2E4[2 * i] + 0x8000;
		particle->oVelY		= sins(D_8032F2E4[2 * i + 1]) * 25.0f;
		particle->oForwardVel	= coss(D_8032F2E4[2 * i + 1]) * 25.0f;
	}
}

void bhv_pound_tiny_star_particle_loop(void)
{
	if(o->oTimer == 0)
	{
		o->oCollisionParticleUnkF4 = 0.28f;
		o->oForwardVel		   = 25.0f;
		o->oPosY -= 20.0f;
		o->oVelY = 14.0f;
	}
	s_optionmove_F();
	o->oAnimState = 4;
	s_set_scale(o->oCollisionParticleUnkF4);
	o->oCollisionParticleUnkF4 -= 0.015f;
}

void bhv_pound_tiny_star_particle_init(void)
{
	s32 sp24;
	s32 sp20 = 8;
	struct Object* particle;
	for(sp24 = 0; sp24 < sp20; sp24++)
	{
		particle		= s_makeobj_nowpos(o, MODEL_CARTOON_STAR, sm64::bhv::bhvPoundTinyStarParticle());
		particle->oMoveAngleYaw = (sp24 * 65536) / sp20;
	}
}
