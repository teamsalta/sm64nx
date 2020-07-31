// tower_door.c.inc

void bhv_tower_door_loop(void)
{
	if(o->oTimer == 0)
		o->oMoveAngleYaw -= 0x4000;
	if(func_802A9A0C(0))
	{
		s_burn_remove(80.0f, 0);
		obj_remove_sound(SOUND_GENERAL_WALL_EXPLOSION);
	}
}
