// castle_cannon_grate.inc.c

void bhv_castle_cannon_grate_init(void)
{
	if(BuGetSumStars(activePlayerNo - 1, 0, 24) >= 120)
		o->activeFlags = 0;
}
