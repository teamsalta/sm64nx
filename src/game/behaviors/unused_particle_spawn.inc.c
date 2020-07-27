/**
 * Behavior for sm64::bhv::bhvUnusedParticleSpawn().
 *
 * This size behavior spawns 10 purple particles if Mario collides with its
 * associated object.
 */

void bhv_unused_particle_spawn_loop(void)
{
	s32 i;
	obj_update_floor_and_walls();
	obj_move_standard(78);

	if(o->oMoveFlags & OBJ_MOVE_ON_GROUND)
	{
		s_remove_obj(o);
	}

	if(s_hitcheck(o, gMarioObject))
	{
		s_remove_obj(o);

		for(i = 0; i < 10; i++)
		{
			s_makeobj_nowpos(o, MODEL_PURPLE_MARBLE, sm64::bhv::bhvPurpleParticle());
		}
	}
}
