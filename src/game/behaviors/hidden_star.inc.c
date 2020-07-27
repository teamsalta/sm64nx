// hidden_star.c.inc

void bhv_hidden_star_init(void)
{
	s16 sp36;
	struct Object* sp30;

	sp36 = count_objects_with_behavior(sm64::bhv::bhvHiddenStarTrigger());
	if(sp36 == 0)
	{
		sp30		 = s_makeobj_absolute(o, 0, MODEL_STAR, sm64::bhv::bhvStar(), o->oPosX, o->oPosY, o->oPosZ, 0, 0, 0);
		sp30->oBehParams = o->oBehParams;
		o->activeFlags	 = 0;
	}

	o->oHiddenStarTriggerCounter = 5 - sp36;
}

void bhv_hidden_star_loop(void)
{
	switch(o->oAction)
	{
		case 0:
			if(o->oHiddenStarTriggerCounter == 5)
				o->oAction = 1;
			break;

		case 1:
			if(o->oTimer > 2 * FRAME_RATE_SCALER_INV)
			{
				func_802F1B84(o->oPosX, o->oPosY, o->oPosZ);
				s_kemuri();
				o->activeFlags = 0;
			}
			break;
	}
}

/* TODO: this is likely not a checkpoint but a Secret */
void bhv_hidden_star_trigger_loop(void)
{
	struct Object* hiddenStar;
	if(s_hitcheck(o, gMarioObject) == 1)
	{
		hiddenStar = s_find_obj(sm64::bhv::bhvHiddenStar());
		if(hiddenStar != NULL)
		{
			hiddenStar->oHiddenStarTriggerCounter++;
			if(hiddenStar->oHiddenStarTriggerCounter != 5)
			{
				AppNumber(hiddenStar->oHiddenStarTriggerCounter, 0, 0, 0);
			}

			AudStartSound(SOUND_MENU_COLLECT_SECRET + (((u8)hiddenStar->oHiddenStarTriggerCounter - 1) << 16), gDefaultSoundArgs);
		}

		o->activeFlags = 0;
	}
}

void bhv_bowser_course_red_coin_star_loop(void)
{
	gRedCoinsCollected = o->oHiddenStarTriggerCounter;
	switch(o->oAction)
	{
		case 0:
			if(o->oHiddenStarTriggerCounter == 8)
				o->oAction = 1;
			break;

		case 1:
			if(o->oTimer > 2 * FRAME_RATE_SCALER_INV)
			{
				func_802F1BD4(o->oPosX, o->oPosY, o->oPosZ);
				s_kemuri();
				o->activeFlags = 0;
			}
			break;
	}
}
