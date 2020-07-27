// breakable_wall.c.inc

void bhv_wf_breakable_wall_loop(void)
{
	if(playerWorks->status == ACT_SHOT_FROM_CANNON)
	{
		s_hitON();
		if(s_hitcheck(o, gMarioObject))
		{
			if(obj_has_behavior(sm64::bhv::bhvWfBreakableWallRight()))
				Na_NazoClearBgm();
			obj_remove_sound(SOUND_GENERAL_WALL_EXPLOSION);
			o->oInteractType      = 8;
			o->oDamageOrCoinValue = 1;
			func_802A3C98(80.0f, 0);
		}
	}
	else
		s_hitOFF();
}
