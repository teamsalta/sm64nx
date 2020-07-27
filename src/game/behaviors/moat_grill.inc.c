// moat_grill.c.inc

void bhv_moat_grills_loop(void)
{
	if(BuGetItemFlag() & SAVE_FLAG_MOAT_DRAINED)
		s_change_shape(MODEL_NONE);
	else
		stMainMoveBG();
}
