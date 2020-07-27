// spawn_star_exit.c.inc

void bhv_ccm_touched_star_spawn_loop(void)
{
	if(gCCMEnteredSlide & 1)
	{
		o->oPosY += 100.0f * FRAME_RATE_SCALER;
		o->oPosX = 2780.0f;
		o->oPosZ = 4666.0f;
		s_enemyset_star(2500.0f, -4350.0f, 5750.0f);
		s_remove_obj(o);
	}
}
