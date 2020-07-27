// white_puff.c.inc

void bhv_white_puff_1_loop(void)
{
	f32 sp1C = 0.1f;
	f32 sp18 = 0.5f;
	if(o->oTimer == 0)
	{
		translate_object_xz_random(o, 40.0f);
		o->oPosY += 30.0f * FRAME_RATE_SCALER;
	}

	s_set_scale(o->oTimer / FRAME_RATE_SCALER_INV * sp18 + sp1C);
	o->oOpacity = 50;

	s_optionmove_F();

	if(o->oTimer > 4 * FRAME_RATE_SCALER_INV)
	{
		s_remove_obj(o);
	}
}

void bhv_white_puff_2_loop(void)
{
	if(o->oTimer == 0)
	{
		translate_object_xz_random(o, 40.0f);
	}
}
