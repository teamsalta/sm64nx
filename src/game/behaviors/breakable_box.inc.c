// breakable_box.c.inc

void bhv_breakable_box_loop(void)
{
	s_set_hitparam(o, &sBreakableBoxHitbox);
	s_change_shape(MODEL_BREAKABLE_BOX_SMALL);

	if(o->oTimer == 0)
		func_802B0E74();

	if(s_block_hitcheck() != 0)
	{
		func_802A3C98(46.0f, 1);
		obj_remove_sound(SOUND_GENERAL_BREAK_BOX);
	}
}
