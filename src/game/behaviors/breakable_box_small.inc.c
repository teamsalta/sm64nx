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

void s_takeblock_init(void)
{
	o->oGravity  = 2.5f;
	o->oFriction = 0.99f;
	o->oBuoyancy = 1.4f;
	s_set_scale(0.4f);
	s_set_hitparam(o, &sBreakableBoxSmallHitbox);
	o->oAnimState = 1;
	o->activeFlags |= 0x200;
}

void takeblock_Kemuri(void)
{
	struct Object* sp24 = s_makeobj_nowpos(o, MODEL_SMOKE, sm64::bhv::bhvSmoke());
	sp24->oPosX += (s32)(Randomf() * 80.0f) - 40;
	sp24->oPosZ += (s32)(Randomf() * 80.0f) - 40;
}

void takeblock_Move(void)
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
			takeblock_Kemuri();
		}
	}

	if(sp1E & 2)
	{
		s_kemuri();
		s_boxeffect(20, 138, 0.7f, 3);
		iwa_MakeCoin(o, 3);
		obj_remove_sound(SOUND_GENERAL_BREAK_BOX);
		o->activeFlags = 0;
	}

	ObjDangerCheck(sp1E, sObjFloor);
}

void takeblock_Return(void)
{
	o->oBreakableBoxSmallFramesSinceReleased++;

	// Begin flashing
	if(o->oBreakableBoxSmallFramesSinceReleased > 810 * FRAME_RATE_SCALER_INV)
	{
		if(o->oBreakableBoxSmallFramesSinceReleased & 1)
			o->header.gfx.node.flags |= 0x10;
		else
			o->header.gfx.node.flags &= ~0x10;
	}

	// Despawn, and create a corkbox respawner
	if(o->oBreakableBoxSmallFramesSinceReleased > 900 * FRAME_RATE_SCALER_INV)
	{
		Obj_reset(MODEL_BREAKABLE_BOX_SMALL, sm64::bhv::bhvBreakableBoxSmall(), 3000);
		o->activeFlags = 0;
	}
}

void takeblock_Event(void)
{
	switch(o->oAction)
	{
		case 0:
			takeblock_Move();
			break;

		case 100:
			ObjMeltEvent();
			break;

		case 101:
			o->activeFlags = 0;
			Obj_reset(MODEL_BREAKABLE_BOX_SMALL, sm64::bhv::bhvBreakableBoxSmall(), 3000);
			break;
	}

	if(o->oBreakableBoxSmallReleased == 1)
		takeblock_Return();
}

void takeblock_Drop(void)
{
	s_hitON();
	s_shapeON();
	s_mode_drop();
	o->header.gfx.node.flags &= ~0x10;
	o->oHeldState				 = 0;
	o->oBreakableBoxSmallReleased		 = 1;
	o->oBreakableBoxSmallFramesSinceReleased = 0;
}

void takeblock_Throw(void)
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

void s_takeblock_main(void)
{
	switch(o->oHeldState)
	{
		case 0:
			takeblock_Event();
			break;

		case 1:
			s_shapeOFF();
			s_hitOFF();
			break;

		case 2:
			takeblock_Throw();
			break;

		case 3:
			takeblock_Drop();
			break;
	}

	o->oInteractStatus = 0;
}
