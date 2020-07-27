
void bhv_ddd_pole_init(void)
{
	if(!(BuGetItemFlag() & (SAVE_FLAG_HAVE_KEY_2 | SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR)))
	{
		s_remove_obj(o);
	}
	else
	{
		o->hitboxDownOffset  = 100.0f;
		o->oDDDPoleMaxOffset = 100.0f * o->oBehParams2ndByte;
	}
}

void bhv_ddd_pole_update(void)
{
	if(o->oTimer > 20 * FRAME_RATE_SCALER_INV)
	{
		o->oDDDPoleOffset += o->oDDDPoleVel * FRAME_RATE_SCALER;

		if(clamp_f32(&o->oDDDPoleOffset, 0.0f, o->oDDDPoleMaxOffset))
		{
			o->oDDDPoleVel = -o->oDDDPoleVel;
			o->oTimer      = 0;
		}
	}

	obj_set_dist_from_home(o->oDDDPoleOffset);
}
