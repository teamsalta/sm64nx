// water_mist.c.inc
// TODO: there is confusion with the earlier mist file. Clarify?

void bhv_water_mist_2_loop(void)
{
	o->oPosY    = mcWaterCheck(o->oHomeX, o->oHomeZ) + 20.0f;
	o->oPosX    = o->oHomeX + s_random_f(150.0f);
	o->oPosZ    = o->oHomeZ + s_random_f(150.0f);
	o->oOpacity = Randomf() * 50.0f + 200.0f;
}
