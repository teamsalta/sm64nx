// exclamation_box.c.inc
#include "game/motor.h"

struct ObjectHitbox sExclamationBoxHitbox = {
    /* interactType: */ INTERACT_BREAKABLE,
    /* downOffset: */ 5,
    /* damageOrCoinValue: */ 0,
    /* health: */ 1,
    /* numLootCoins: */ 0,
    /* radius: */ 40,
    /* height: */ 30,
    /* hurtboxRadius: */ 40,
    /* hurtboxHeight: */ 30,
};

struct Struct802C0DF0 sExclamationBoxContents[] = {
    {0, 0, 0, MODEL_MARIOS_WING_CAP, sm64::bhv::bhvWingCap()},
    {1, 0, 0, MODEL_MARIOS_METAL_CAP, sm64::bhv::bhvMetalCap()},
    {2, 0, 0, MODEL_MARIOS_CAP, sm64::bhv::bhvVanishCap()},
    {3, 0, 0, MODEL_KOOPA_SHELL, sm64::bhv::bhvKoopaShell()},
    {4, 0, 0, MODEL_YELLOW_COIN, sm64::bhv::bhvSingleCoinGetsSpawned()},
    {5, 0, 0, MODEL_NONE, sm64::bhv::bhvThreeCoinsSpawn()},
    {6, 0, 0, MODEL_NONE, sm64::bhv::bhvTenCoinsSpawn()},
    {7, 0, 0, MODEL_1UP, sm64::bhv::bhv1upWalking()},
    {8, 0, 0, MODEL_STAR, sm64::bhv::bhvSpawnedStar()},
    {9, 0, 0, MODEL_1UP, sm64::bhv::bhv1upRunningAway()},
    {10, 0, 1, MODEL_STAR, sm64::bhv::bhvSpawnedStar()},
    {11, 0, 2, MODEL_STAR, sm64::bhv::bhvSpawnedStar()},
    {12, 0, 3, MODEL_STAR, sm64::bhv::bhvSpawnedStar()},
    {13, 0, 4, MODEL_STAR, sm64::bhv::bhvSpawnedStar()},
    {14, 0, 5, MODEL_STAR, sm64::bhv::bhvSpawnedStar()},
    {99, 0, 0, 0, NULL}};

void bhv_rotatin_exclamation_box_loop(void)
{
	if(o->parentObj->oAction != 1)
		s_remove_obj(o);
}

static void itembox_init(void)
{
	if(o->oBehParams2ndByte < 3)
	{
		o->oAnimState = o->oBehParams2ndByte;
		if((BuGetItemFlag() & D_8032F0C0[o->oBehParams2ndByte]) || ((o->oBehParams >> 24) & 0xFF) != 0)
			o->oAction = 2;
		else
			o->oAction = 1;
	}
	else
	{
		o->oAnimState = 3;
		o->oAction    = 2;
	}
}

static void itembox_disable(void)
{
	s_hitOFF();
	if(o->oTimer == 0)
	{
		s_makeobj_nowpos(o, MODEL_EXCLAMATION_POINT, sm64::bhv::bhvRotatingExclamationMark());
		s_change_shape(MODEL_EXCLAMATION_BOX_OUTLINE);
	}
	if((BuGetItemFlag() & D_8032F0C0[o->oBehParams2ndByte]) || ((o->oBehParams >> 24) & 0xFF) != 0)
	{
		o->oAction = 2;
		s_change_shape(MODEL_EXCLAMATION_BOX);
	}
}

static void itembox_enable(void)
{
	s_set_hitparam(o, &sExclamationBoxHitbox);
	if(o->oTimer == 0)
	{
		s_shape_disp();
		s_hitON();
		o->oInteractStatus = 0;
		o->oPosY	   = o->oHomeY;
		o->oGraphYOffset   = 0.0f;
	}
	if(s_block_hitcheck())
	{
		s_hitOFF();
		o->oExclamationBoxUnkFC = 0x4000;
		o->oVelY		= 30.0f;
		o->oGravity		= -8.0f;
		o->oFloorHeight		= o->oPosY;
		o->oAction		= 3;

		SendMotorEvent(5, 80);
	}
	stMainMoveBG();
}

static void itembox_burn(void)
{
	s_optionmove_F();

	if(o->oVelY < 0.0f)
	{
		o->oVelY    = 0.0f;
		o->oGravity = 0.0f;
	}

	o->oExclamationBoxUnkF8 = (sins(o->oExclamationBoxUnkFC) + 1.0) * 0.3 + 0.0;
	o->oExclamationBoxUnkF4 = (-sins(o->oExclamationBoxUnkFC) + 1.0) * 0.5 + 1.0;
	o->oGraphYOffset	= (-sins(o->oExclamationBoxUnkFC) + 1.0) * 26.0;
	o->oExclamationBoxUnkFC += 0x1000 / FRAME_RATE_SCALER_INV;
	o->header.gfx.scale[0] = o->oExclamationBoxUnkF4 * 2.0f;
	o->header.gfx.scale[1] = o->oExclamationBoxUnkF8 * 2.0f;
	o->header.gfx.scale[2] = o->oExclamationBoxUnkF4 * 2.0f;
	if(o->oTimer == 7 * FRAME_RATE_SCALER_INV)
		o->oAction = 4;
}

static void s_itembox_makeobj(struct Struct802C0DF0* a0, u8 a1)
{
	Object* stp = NULL;

	while(a0->unk0 != 99)
	{
		if(a1 == a0->unk0)
		{
			stp		   = s_makeobj_nowpos(o, a0->model, a0->behavior);
			stp->oVelY	   = 20.0f;
			stp->oForwardVel   = 3.0f;
			stp->oMoveAngleYaw = gMarioObject->oMoveAngleYaw;
			o->oBehParams |= a0->unk2 << 24;
			if(a0->model == 122)
				o->oFlags |= 0x4000;
			break;
		}
		a0++;
	}
}

void itembox_makeitem(void)
{
	s_itembox_makeobj(sExclamationBoxContents, o->oBehParams2ndByte);
	s_burneffect(0, 0, 46.0f);
	s_boxeffect(20, 139, 0.3f, o->oAnimState);
	obj_remove_sound(SOUND_GENERAL_BREAK_BOX);

	if(o->oBehParams2ndByte < 3)
	{
		o->oAction = 5;
		s_shape_hide();
	}
	else
	{
		s_remove_obj(o);
	}
}

void itembox_return(void)
{
	if(o->oTimer > 300 * FRAME_RATE_SCALER_INV)
	{
		o->oAction = 2;
	}
}

void (*itembox_modejmp[])(void) = {itembox_init, itembox_disable, itembox_enable, itembox_burn, itembox_makeitem, itembox_return};

void bhv_exclamation_box_loop(void)
{
	s_set_scale(2.0f);
	s_modejmp(itembox_modejmp);
}
