// ddd_sub.c.inc

void bhv_bowsers_sub_loop(void)
{
	if(BuGetItemFlag() & (SAVE_FLAG_HAVE_KEY_2 | SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR))
		s_remove_obj(o);
}
