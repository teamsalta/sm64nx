// breakable_box.c.inc

struct ObjectHitbox sBreakableBoxSmallHitbox = {
    /* interactType:      */ INTERACT_GRABBABLE,
    /* downOffset:        */ 20,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 150,
    /* height:            */ 250,
    /* hurtboxRadius:     */ 150,
    /* hurtboxHeight:     */ 250,
};

void bhv_breakable_box_small_init(void)
{
	o->oGravity  = 2.5f;
	o->oFriction = 0.99f;
	o->oBuoyancy = 1.4f;
	s_set_scale(0.4f);
	s_set_hitparam(o, &sBreakableBoxSmallHitbox);
	o->oAnimState = 1;
	o->activeFlags |= 0x200;
}

void func_802F4CE8(void)
{
	struct Object* sp24 = s_makeobj_nowpos(o, MODEL_SMOKE, sm64::bhv::bhvSmoke());
	sp24->oPosX += (s32)(Randomf() * 80.0f) - 40;
	sp24->oPosZ += (s32)(Randomf() * 80.0f) - 40;
}

void func_802F4DB4(void)
{
	s16 sp1E = ObjMoveEvent();

	s_fire_hitcheck(o);
	if(sp1E == 1)
		objsound(SOUND_GENERAL_BOX_LANDING_2);

	if(sp1E & 1)
	{
		if(o->oForwardVel > 20.0f)
		{
			objsound(SOUND_ENV_SLIDING);
			func_802F4CE8();
		}
	}

	if(sp1E & 2)
	{
		s_kemuri();
		s_boxeffect(20, 138, 0.7f, 3);
		obj_spawn_yellow_coins(o, 3);
		obj_remove_sound(SOUND_GENERAL_BREAK_BOX);
		o->activeFlags = 0;
	}

	obj_check_floor_death(sp1E, sObjFloor);
}

void breakable_box_small_released_loop(void)
{
	o->oBreakableBoxSmallFramesSinceReleased++;

	// Begin flashing
	if(o->oBreakableBoxSmallFramesSinceReleased > 810)
	{
		if(o->oBreakableBoxSmallFramesSinceReleased & 1)
			o->header.gfx.node.flags |= 0x10;
		else
			o->header.gfx.node.flags &= ~0x10;
	}

	// Despawn, and create a corkbox respawner
	if(o->oBreakableBoxSmallFramesSinceReleased > 900)
	{
		create_respawner(MODEL_BREAKABLE_BOX_SMALL, sm64::bhv::bhvBreakableBoxSmall(), 3000);
		o->activeFlags = 0;
	}
}

void breakable_box_small_idle_loop(void)
{
	switch(o->oAction)
	{
		case 0:
			func_802F4DB4();
			break;

		case 100:
			obj_lava_death();
			break;

		case 101:
			o->activeFlags = 0;
			create_respawner(MODEL_BREAKABLE_BOX_SMALL, sm64::bhv::bhvBreakableBoxSmall(), 3000);
			break;
	}

	if(o->oBreakableBoxSmallReleased == 1)
		breakable_box_small_released_loop();
}

void breakable_box_small_get_dropped(void)
{
	s_hitON();
	s_shapeON();
	s_mode_drop();
	o->header.gfx.node.flags &= ~0x10;
	o->oHeldState				 = 0;
	o->oBreakableBoxSmallReleased		 = 1;
	o->oBreakableBoxSmallFramesSinceReleased = 0;
}

void breakable_box_small_get_thrown(void)
{
	s_hitON();
	s_throw_object();
	s_shapeON();
	o->header.gfx.node.flags &= ~0x10;
	o->oHeldState = 0;
	o->oFlags &= ~0x08;
	o->oForwardVel				 = 40.0f;
	o->oVelY				 = 20.0f;
	o->oBreakableBoxSmallReleased		 = 1;
	o->oBreakableBoxSmallFramesSinceReleased = 0;
	o->activeFlags &= ~0x200;
}

void bhv_breakable_box_small_loop(void)
{
	switch(o->oHeldState)
	{
		case 0:
			breakable_box_small_idle_loop();
			break;

		case 1:
			s_shapeOFF();
			s_hitOFF();
			break;

		case 2:
			breakable_box_small_get_thrown();
			break;

		case 3:
			breakable_box_small_get_dropped();
			break;
	}

	o->oInteractStatus = 0;
}
