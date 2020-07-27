// koopa_shell_underwater.c.inc

struct ObjectHitbox sKoopaShellUnderwaterHitbox = {
    /* interactType: */ INTERACT_GRABBABLE,
    /* downOffset: */ 0,
    /* damageOrCoinValue: */ 0,
    /* health: */ 1,
    /* numLootCoins: */ 0,
    /* radius: */ 80,
    /* height: */ 50,
    /* hurtboxRadius: */ 0,
    /* hurtboxHeight: */ 0,
};

void func_802AA0D4(void)
{
	s_set_hitparam(o, &sKoopaShellUnderwaterHitbox);
}

void bhv_koopa_shell_underwater_loop(void)
{
	switch(o->oHeldState)
	{
		case HELD_FREE:
			func_802AA0D4();
			break;
		case HELD_HELD:
			func_8029FA5C(-1, 0);
			break;
		case HELD_THROWN:
		case HELD_DROPPED:
			s_remove_obj(o);
			s_kemuri();
			break;
	}
	if(o->oInteractStatus & INT_STATUS_STOP_RIDING)
	{
		s_remove_obj(o);
		s_kemuri();
	}
	o->oInteractStatus = 0;
}
