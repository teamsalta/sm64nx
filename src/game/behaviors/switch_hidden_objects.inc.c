// switch_hidden_objects.c.inc

struct ObjectHitbox sBreakableBoxHitbox = {
    /* interactType: */ INTERACT_BREAKABLE,
    /* downOffset: */ 20,
    /* damageOrCoinValue: */ 0,
    /* health: */ 1,
    /* numLootCoins: */ 0,
    /* radius: */ 150,
    /* height: */ 200,
    /* hurtboxRadius: */ 150,
    /* hurtboxHeight: */ 200,
};

void func_802B0E74(void)
{
	o->oPurpleSwitch = NULL;
	o->oAnimState	 = 1;
	switch(o->oBehParams2ndByte)
	{
		case 0:
			o->oNumLootCoins = 0;
			break;
		case 1:
			o->oNumLootCoins = 3;
			break;
		case 2:
			o->oNumLootCoins = 5;
			break;
		case 3:
			s_set_scale(1.5f);
			break;
	}
}

void func_802B0F54(void)
{
	struct Object* purpleSwitch;
	s_set_hitparam(o, &sBreakableBoxHitbox);
	s_change_shape(MODEL_BREAKABLE_BOX_SMALL);

	if(o->oAction == 0)
	{
		s_shapeOFF();
		s_hitOFF();

		if(o->oTimer == 0)
		{
			func_802B0E74();
		}

		if(o->oPurpleSwitch == NULL)
		{
			o->oPurpleSwitch = s_find_obj(sm64::bhv::bhvFloorSwitchHiddenObjects());
		}

		if((purpleSwitch = o->oPurpleSwitch) != NULL)
		{
			if(purpleSwitch->oAction == PURPLE_SWITCH_TICKING)
			{
				o->oAction++;
				s_shapeON();
				s_shape_disp();
			}
		}
	}
	else if(o->oAction == 1)
	{
		s_hitON();

		if(s_flash_shape(360 * FRAME_RATE_SCALER_INV, 20))
		{
			o->oAction = 0;
		}

		if(s_block_hitcheck())
		{
			s_kemuri();
			s_boxeffect(30, 138, 3.0f, 4);
			o->oAction++;
			objsound(SOUND_GENERAL_BREAK_BOX);
		}
		stMainMoveBG();
	}
	else
	{
		s_hitOFF();
		s_shapeOFF();
		o->oInteractStatus = 0;
		if((purpleSwitch = o->oPurpleSwitch) != NULL)
		{
			if(purpleSwitch->oAction == 0)
				o->oAction = 0;
		}
	}
}

void func_802B1138(void)
{
	struct Object* purpleSwitch;

	s_set_shapeinfo(o, wdw_seg7_collision_07018528);

	if(o->oAction == 0)
	{
		s_shapeOFF();
		s_hitOFF();

		if(o->oPurpleSwitch == NULL)
		{
			o->oPurpleSwitch = s_find_obj(sm64::bhv::bhvFloorSwitchHiddenObjects());
		}

		if((purpleSwitch = o->oPurpleSwitch) != NULL)
		{
			if(purpleSwitch->oAction == 2)
			{
				o->oAction++;
				s_shapeON();
				s_shape_disp();
			}
		}
	}
	else
	{
		s_hitON();

		if(s_flash_shape(360 * FRAME_RATE_SCALER_INV, 20))
		{
			o->oAction = 0;
		}

		stMainMoveBG();
	}
}

void bhv_hidden_object_loop(void)
{
	if(o->oBehParams2ndByte == 0)
	{
		func_802B0F54(); // Confused, that function has code depending on the action
	}
	else
	{
		func_802B1138();
	}
}
