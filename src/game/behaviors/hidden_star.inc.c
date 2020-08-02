// hidden_star.c.inc

void s_getdummy_star_init(void)
{
	s16 sp36;
	struct Object* sp30;

	sp36 = s_count_obj(sm64::bhv::bhvHiddenStarTrigger());
	if(sp36 == 0)
	{
		sp30		 = s_makeobj_absolute(o, 0, MODEL_STAR, sm64::bhv::bhvStar(), o->oPosX, o->oPosY, o->oPosZ, 0, 0, 0);
		sp30->oBehParams = o->oBehParams;
		o->activeFlags	 = 0;
	}

	o->oHiddenStarTriggerCounter = 5 - sp36;
}

void s_getdummy_star_main(void)
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
				s_coinset_star(o->oPosX, o->oPosY, o->oPosZ);
				s_kemuri();
				o->activeFlags = 0;
			}
			break;
	}
}

/* TODO: this is likely not a checkpoint but a Secret */
void s_dummy_appear(void)
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

void s_getcoins_extstar_main(void)
{
	redcoin_num = o->oHiddenStarTriggerCounter;
	switch(o->oAction)
	{
		case 0:
			if(o->oHiddenStarTriggerCounter == 8)
				o->oAction = 1;
			break;

		case 1:
			if(o->oTimer > 2 * FRAME_RATE_SCALER_INV)
			{
				s_extraset_star(o->oPosX, o->oPosY, o->oPosZ);
				s_kemuri();
				o->activeFlags = 0;
			}
			break;
	}
}
