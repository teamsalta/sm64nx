// treasure_chest.c.inc

#include "game/motor.h"

/**
 * Hitbox for treasure chest bottom.
 */
static struct ObjectHitbox sTreasureChestBottomHitbox = {
    /* interactType:      */ INTERACT_SHOCK,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 300,
    /* height:            */ 300,
    /* hurtboxRadius:     */ 310,
    /* hurtboxHeight:     */ 310,
};

void bhv_treasure_chest_top_loop()
{
	struct Object* sp34 = o->parentObj->parentObj;

	switch(o->oAction)
	{
		case 0:
			if(o->parentObj->oAction == 1)
				o->oAction = 1;
			break;

		case 1:
			if(o->oTimer == 0)
			{
				if(sp34->oTreasureChestUnkFC == 0)
				{
					s_makeobj_chain(0, 0, -80, 120, o, MODEL_BUBBLE, sm64::bhv::bhvWaterAirBubble());
					AudStartSound(SOUND_GENERAL_CLAM_SHELL1, o->header.gfx.cameraToObject);
				}
				else
				{
					AudStartSound(SOUND_GENERAL_OPEN_CHEST, o->header.gfx.cameraToObject);
				}
			}

			o->oFaceAnglePitch += -0x200 / FRAME_RATE_SCALER_INV;
			if(o->oFaceAnglePitch < -0x4000)
			{
				o->oFaceAnglePitch = -0x4000;
				o->oAction++;

				if(o->parentObj->oBehParams2ndByte != 4)
					AppNumber(o->parentObj->oBehParams2ndByte, 0, -40, 0);
			}
			break;

		case 2:
			if(o->parentObj->oAction == 0)
				o->oAction = 3;
			break;

		case 3:
			o->oFaceAnglePitch += 0x800 / FRAME_RATE_SCALER_INV;
			if(o->oFaceAnglePitch > 0)
			{
				o->oFaceAnglePitch = 0;
				o->oAction	   = 0;
			}
	}
}

void bhv_treasure_chest_bottom_init()
{
	s_makeobj_chain(0, 0, 102, -77, o, MODEL_TREASURE_CHEST_LID, sm64::bhv::bhvTreasureChestTop());
	s_set_hitparam(o, &sTreasureChestBottomHitbox);
}

void bhv_treasure_chest_bottom_loop()
{
	switch(o->oAction)
	{
		case 0:
			if(ShapeSameAngle(o->oMoveAngleYaw, gMarioObject->header.gfx.angle[1] + 0x8000, 0x3000))
			{
				if(PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 150))
				{
					if(!o->parentObj->oTreasureChestUnkF8)
					{
						if(o->parentObj->oTreasureChestUnkF4 == o->oBehParams2ndByte)
						{
							AudStartSound(SOUND_GENERAL2_RIGHT_ANSWER, gDefaultSoundArgs);
							o->parentObj->oTreasureChestUnkF4++;
							o->oAction = 1;
						}
						else
						{
							o->parentObj->oTreasureChestUnkF4 = 1;
							o->parentObj->oTreasureChestUnkF8 = 1;
							o->oAction			  = 2;
							s_hitON();
							AudStartSound(SOUND_MENU_CAMERA_BUZZ, gDefaultSoundArgs);
						}
					}
				}
			}
			break;

		case 1:
			if(o->parentObj->oTreasureChestUnkF8 == 1)
				o->oAction = 0;
			break;

		case 2:
			s_hitOFF();
			if(!PlayerApproach(o->oPosX, o->oPosY, o->oPosZ, 500))
			{
				o->parentObj->oTreasureChestUnkF8 = 0;
				o->oAction			  = 0;
			}
	}

	s_player_slideout_RH(150.0f, 150.0f);
	o->oInteractStatus = 0;
}

void tBoxMake(s8 sp3B, s32 sp3C, s32 sp40, s32 sp44, s16 sp4A)
{
	struct Object* sp34;
	sp34			= s_makeobj_absolute(o, 0, MODEL_TREASURE_CHEST_BASE, sm64::bhv::bhvTreasureChestBottom(), sp3C, sp40, sp44, 0, sp4A, 0);
	sp34->oBehParams2ndByte = sp3B;
}

void bhv_treasure_chest_ship_init()
{
	tBoxMake(1, 400, -350, -2700, 0);
	tBoxMake(2, 650, -350, -940, -0x6001);
	tBoxMake(3, -550, -350, -770, 0x5FFF);
	tBoxMake(4, 100, -350, -1700, 0);
	o->oTreasureChestUnkF4 = 1;
	o->oTreasureChestUnkFC = 0;
}

void bhv_treasure_chest_ship_loop()
{
	switch(o->oAction)
	{
		case 0:
			if(o->oTreasureChestUnkF4 == 5)
			{
				Na_NazoClearBgm();
				Na_AllSeqSubVolumeTrans(0, 127, 1000);
				o->oAction = 1;
			}
			break;

		case 1:
			if(gEnvironmentRegions != NULL)
			{
				gEnvironmentRegions[6] += -5;
				AudStartSound(SOUND_ENV_WATER_DRAIN, gDefaultSoundArgs);
				Viewshake(SHAKE_ENV_JRB_SHIP_DRAIN);
				if(gEnvironmentRegions[6] < -335)
				{
					gEnvironmentRegions[6] = -335;
					o->activeFlags	       = 0;
				}
				SendMotorVib(2);
			}
			break;
	}
}

void bhv_treasure_chest_jrb_init()
{
	tBoxMake(1, -1700, -2812, -1150, 0x7FFF);
	tBoxMake(2, -1150, -2812, -1550, 0x7FFF);
	tBoxMake(3, -2400, -2812, -1800, 0x7FFF);
	tBoxMake(4, -1800, -2812, -2100, 0x7FFF);
	o->oTreasureChestUnkF4 = 1;
	o->oTreasureChestUnkFC = 1;
}

void bhv_treasure_chest_jrb_loop()
{
	switch(o->oAction)
	{
		case 0:
			if(o->oTreasureChestUnkF4 == 5)
			{
				Na_NazoClearBgm();
				o->oAction = 1;
			}
			break;

		case 1:
			if(o->oTimer == 60 * FRAME_RATE_SCALER_INV)
			{
				s_kemuri();
				s_enemyset_star(-1800.0f, -2500.0f, -1700.0f);
				o->oAction = 2;
			}
			break;

		case 2:
			break;
	}
}

void s_tbox_quize23_init()
{
	tBoxMake(1, -4500, -5119, 1300, -0x6001);
	tBoxMake(2, -1800, -5119, 1050, 0x1FFF);
	tBoxMake(3, -4500, -5119, -1100, 9102);
	tBoxMake(4, -2400, -4607, 125, 16019);

	o->oTreasureChestUnkF4 = 1;
	o->oTreasureChestUnkFC = 0;
}

void s_tbox_quize23_main()
{
	switch(o->oAction)
	{
		case 0:
			if(o->oTreasureChestUnkF4 == 5)
			{
				Na_NazoClearBgm();
				o->oAction = 1;
			}
			break;

		case 1:
			if(o->oTimer == 60 * FRAME_RATE_SCALER_INV)
			{
				s_kemuri();
				s_enemyset_star(-1900.0f, -4000.0f, -1400.0f);
				o->oAction = 2;
			}
			break;

		case 2:
			break;
	}
}
