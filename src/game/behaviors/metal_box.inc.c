// metal_box.c.inc

struct ObjectHitbox sMetalBoxHitbox = {
    /* interactType: */ 0,
    /* downOffset: */ 0,
    /* damageOrCoinValue: */ 0,
    /* health: */ 1,
    /* numLootCoins: */ 0,
    /* radius: */ 220,
    /* height: */ 300,
    /* hurtboxRadius: */ 220,
    /* hurtboxHeight: */ 300,
};

s32 func_802B0C54(f32 a0, f32 a1)
{
	struct Surface* sp24;
	f32 sp20 = o->oPosX + sins(o->oMoveAngleYaw) * a1;
	f32 floorHeight;
	f32 sp18    = o->oPosZ + coss(o->oMoveAngleYaw) * a1;
	floorHeight = find_floor(sp20, o->oPosY, sp18, &sp24);
	if(absf(floorHeight - o->oPosY) < a0) // abs
		return 1;
	else
		return 0;
}

void bhv_pushable_loop(void)
{
	UNUSED s16 unused;
	s16 sp1C;
	s_set_hitparam(o, &sMetalBoxHitbox);
	o->oForwardVel = 0.0f;
	if(s_hitcheck(o, gMarioObject) && playerWorks->flags & 0x80000000)
	{
		sp1C = angle_to_object(o, gMarioObject);
		if(abs_angle_diff(sp1C, gMarioObject->oMoveAngleYaw) > 0x4000)
		{
			o->oMoveAngleYaw = (s16)((gMarioObject->oMoveAngleYaw + 0x2000) & 0xc000);
			if(func_802B0C54(8.0f, 150.0f))
			{
				o->oForwardVel = 4.0f;
				objsound_level(SOUND_ENV_METAL_BOX_PUSH);
			}
		}
	}
	s_optionmove_F();
}
