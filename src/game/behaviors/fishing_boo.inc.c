// fishing_boo.c.inc

// Unused behavior file for the Fishing Boo enemy.
// The fire circles around mario and would make it
// hard for him to jump or platform.
// There is also a stubbed behavior script next to
// the scripts associated with this, indicating
// a removed object associated with this flame.

void bhv_beta_moving_flames_spawn_loop(void)
{
	o->oDistanceToMario = s_distanceXZ_obj2obj(o, gMarioObject);
	o->oPosY -= 100.0f * FRAME_RATE_SCALER;

	switch(o->oAction)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			s_makeobj_nowpos(o, MODEL_RED_FLAME, sm64::bhv::bhvBetaMovingFlames());
			o->oAction++;
			break;
		case 8:
			break;
		case 9:
			o->oAction++;
			break;
	}
}

void bhv_beta_moving_flames_loop(void)
{
	s_set_scale(5.0f);
	o->oForwardVel = sins(o->oMovingFlameTimer) * 70.0f;
	o->oMovingFlameTimer += 0x800 / FRAME_RATE_SCALER_INV;
}
