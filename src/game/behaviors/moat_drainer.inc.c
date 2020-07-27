// moat_drainer.c.inc

void s_maincastle_waterswitch(void)
{
	if(BuGetItemFlag() & SAVE_FLAG_MOAT_DRAINED)
	{
		gEnvironmentRegions[6]	= -800;
		gEnvironmentRegions[12] = -800;
	}
}
