// water_pillar.c.inc
#include "game/motor.h"

static void sel1b_erase_water_1(void)
{
	struct Object* sp1C;
	switch(o->oAction)
	{
		case 0:
			if(s_checkplayer_hipaatack())
			{
				o->oAction++;
				s_kemuri();
			}
			break;
		case 1:
			if(o->oTimer < 4 * FRAME_RATE_SCALER_INV)
				o->oPosY -= 20.0f;
			else
				o->oAction++;
			break;
		case 2:
			sp1C = s_find_obj(sm64::bhv::bhvWaterLevelPillar());
			if(sp1C != NULL)
			{
				if(sp1C->oAction < 2)
					o->oAction++;
			}
			break;
		case 3:
			sp1C = s_find_obj(sm64::bhv::bhvWaterLevelPillar());
			if(sp1C != NULL)
			{
				if(sp1C->oAction > 1)
				{
					o->oAction++;

					BuSetItemFlag(SAVE_FLAG_MOAT_DRAINED);
					Na_NazoClearBgm();
				}
			}
			break;
		case 4:
			objsound_level(SOUND_ENV_WATER_DRAIN);
			if(o->oTimer < 300 * FRAME_RATE_SCALER_INV)
			{
				gEnvironmentLevels[2] = (s32)approach_f32_symmetric(gEnvironmentLevels[2], -2450.0f, 5.0f * FRAME_RATE_SCALER);
				gEnvironmentLevels[0] = (s32)approach_f32_symmetric(gEnvironmentLevels[0], -2450.0f, 5.0f * FRAME_RATE_SCALER);
				SendMotorVib(2);
			}
			else
				o->oAction++;
			break;
		case 5:
			break;
	}
}

void sel1b_erase_water_2(void)
{
	if(o->oTimer == 0)
	{
		o->oPosY -= 80.0f;
		gEnvironmentLevels[2] = -2450;
		gEnvironmentLevels[0] = -2450;
	}
}

void bhv_water_level_pillar_init(void)
{
	if(BuGetItemFlag() & SAVE_FLAG_MOAT_DRAINED)
		o->oWaterLevelPillarUnkF8 = 1;
}

void bhv_water_level_pillar_loop(void)
{
	if(o->oWaterLevelPillarUnkF8)
		sel1b_erase_water_2();
	else
		sel1b_erase_water_1();
	gEnvironmentRegions[18] = gEnvironmentLevels[2];
	gEnvironmentRegions[6]	= gEnvironmentLevels[0];
}
