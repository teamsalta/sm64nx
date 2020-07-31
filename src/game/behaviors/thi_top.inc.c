// thi_top.c.inc

struct SpawnParticlesInfo D_8032F134 = {0, 30, MODEL_WHITE_PARTICLE_SMALL, 0, 40, 0, 20, 40, 252, 30, 20.0f, 0.0f};

UNUSED u8 unused8032F134[] = {10, 11, 12};

void bhv_thi_huge_island_top_loop(void)
{
	if(gTHIWaterDrained & 1)
	{
		if(o->oTimer == 0)
			gEnvironmentRegions[18] = 3000;
		s_shape_hide();
	}
	else
		stMainMoveBG();
}

void bhv_thi_tiny_island_top_loop(void)
{
	if(!(gTHIWaterDrained & 1))
	{
		if(o->oAction == 0)
		{
			if(o->oDistanceToMario < 500.0f)
				if(playerWorks->status == ACT_GROUND_POUND_LAND)
				{
					o->oAction++;
					s_makeeffect_chiri(&D_8032F134);
					s_boxeffect(20, 138, 0.3f, 3);
					objsound(SOUND_GENERAL_ACTIVATE_CAP_SWITCH);
					s_shape_hide();
				}
		}
		else
		{
			if(o->oTimer < 50 * FRAME_RATE_SCALER_INV)
			{
				gEnvironmentRegions[18]--;
				objsound_level(SOUND_ENV_WATER_DRAIN);
			}
			else
			{
				gTHIWaterDrained |= 1;
				Na_NazoClearBgm();
				o->oAction += 1;
			}
		}
	}
	else
	{
		if(o->oTimer == 0)
			gEnvironmentRegions[18] = 700;
		s_shape_hide();
	}
}
