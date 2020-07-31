// koopa_shell.c.inc

struct ObjectHitbox sKoopaShellHitbox = {
    /* interactType: */ INTERACT_KOOPA_SHELL,
    /* downOffset: */ 0,
    /* damageOrCoinValue: */ 4,
    /* health: */ 1,
    /* numLootCoins: */ 1,
    /* radius: */ 50,
    /* height: */ 50,
    /* hurtboxRadius: */ 50,
    /* hurtboxHeight: */ 50,
};

void func_802BCA8C(void)
{
	UNUSED s32 unused;
	struct Object* drop;
	s_makeobj_nowpos(o, MODEL_WATER_WAVES, sm64::bhv::bhvWaterType());
	if(playerWorks->forwardVel > 10.0f)
	{
		drop	    = s_makeobj_nowpos_scale(o, MODEL_WHITE_PARTICLE_SMALL, sm64::bhv::bhvWaterDrops(), 1.5f);
		drop->oVelY = Randomf() * 30.0f;
		s_random_XZ_offset(drop, 110.0f);
	}
}

void bhv_koopa_shell_flame_loop(void)
{
	if(o->oTimer == 0)
	{
		o->oMoveAngleYaw = RandomU16();
		o->oVelY	 = Randomf() * 30.0f;
		o->oGravity	 = -4.0f;
		o->oAnimState	 = Randomf() * 10.0f;
		s_random_XZ_offset(o, 110.0f);
		o->oKoopaShellFlameUnkF8 = 4.0f;
	}
	s_groundcheck();
	s_optionmove_F();
	if(o->oFloorHeight > o->oPosY || o->oTimer > 10 * FRAME_RATE_SCALER_INV)
		s_remove_obj(o);
	o->oKoopaShellFlameUnkF8 += -0.3 * FRAME_RATE_SCALER;
	s_set_scale(o->oKoopaShellFlameUnkF8);
}

void bhv_koopa_shell_flame_spawn(void)
{
	s32 i;
	for(i = 0; i < 2; i++)
		s_makeobj_nowpos(o, MODEL_RED_FLAME, sm64::bhv::bhvKoopaShellFlame());
}

void func_802BCCD4(f32 a)
{
	struct Object* sp1C = s_makeobj_nowpos(o, MODEL_NONE, sm64::bhv::bhvSparkleSpawn());
	sp1C->oPosY += a * FRAME_RATE_SCALER;
}

void bhv_koopa_shell_loop(void)
{
	struct Surface* sp34;
	s_set_hitparam(o, &sKoopaShellHitbox);
	s_set_scale(1.0f);
	switch(o->oAction)
	{
		case 0:
			s_enemybgcheck();
			s_wallreverse();
			if(o->oInteractStatus & INT_STATUS_INTERACTED)
				o->oAction++;
			o->oFaceAngleYaw += 0x1000 / FRAME_RATE_SCALER_INV;
			s_enemymove(-20);
			func_802BCCD4(10.0f);
			break;
		case 1:
			s_copy_worldXYZ(o, gMarioObject);
			sp34 = s_groundcheck_ptr();
			if(s_abs_f(find_water_level(o->oPosX, o->oPosZ) - o->oPosY) < 10.0f)
				func_802BCA8C();
			else if(5.0f > s_abs_f(o->oPosY - o->oFloorHeight))
			{
				if(sp34 != NULL && sp34->type == 1)
					bhv_koopa_shell_flame_spawn();
				else
					func_802BCCD4(10.0f);
			}
			else
				func_802BCCD4(10.0f);
			o->oFaceAngleYaw = gMarioObject->oMoveAngleYaw;
			if(o->oInteractStatus & INT_STATUS_STOP_RIDING)
			{
				s_remove_obj(o);
				s_kemuri();
				o->oAction = 0;
			}
			break;
	}
	o->oInteractStatus = 0;
}
