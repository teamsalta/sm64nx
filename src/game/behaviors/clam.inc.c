// clam.inc.c

struct ObjectHitbox sClamShellHitbox = {
    /* interactType:      */ INTERACT_CLAM_OR_BUBBA,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 99,
    /* numLootCoins:      */ 0,
    /* radius:            */ 150,
    /* height:            */ 80,
    /* hurtboxRadius:     */ 150,
    /* hurtboxHeight:     */ 80,
};

void clam_act_0(void)
{
	if(func_802F92EC(0, 25))
	{
		objsound(SOUND_GENERAL_CLAM_SHELL3);
		func_802ADA94();
		s_hitON();

		o->oClamUnkF4 = 10;
		o->oTimer     = 0;
	}
	else if(o->oTimer > 150 * FRAME_RATE_SCALER_INV && o->oDistanceToMario < 500.0f)
	{
		objsound(SOUND_GENERAL_CLAM_SHELL2);
		o->oAction = 1;
	}
	else if(o->oClamUnkF4 != 0)
	{
		o->oClamUnkF4 -= 1;
		s_effect_updown(3.0f);
	}
}

void clam_act_1(void)
{
	s16 val06;
	s16 val04;
	s16 val02;

	if(o->oTimer > 150 * FRAME_RATE_SCALER_INV)
	{
		o->oAction = 0;
	}
	else if(obj_is_rendering_enabled() && func_802F92EC(1, 8))
	{
		for(val06 = -0x2000; val06 < 0x2000; val06 += 0x555)
		{
			val04 = (s16)(100.0f * sins(val06));
			val02 = (s16)(100.0f * coss(val06));

			s_makeobj_chain(0, val04, 30, val02, o, MODEL_BUBBLE, sm64::bhv::bhvBubbleMaybe());
		}
	}
	else if(s_check_animenumber(30))
	{
		s_hitOFF();
	}
}

void bhv_clam_loop(void)
{
	o->header.gfx.scale[1] = 1.5f;

	switch(o->oAction)
	{
		case 0:
			clam_act_0();
			break;
		case 1:
			clam_act_1();
			break;
	}

	obj_check_attacks(&sClamShellHitbox, o->oAction);
}
