// explosion.c.inc

void bhv_explosion_init(void)
{
	obj_remove_sound(SOUND_GENERAL2_BOBOMB_EXPLOSION);
	Viewshake(SHAKE_ENV_EXPLOSION);

	o->oOpacity = 255;
}

void bhv_explosion_loop(void)
{
	s32 i;

	if(o->oTimer == 9 * FRAME_RATE_SCALER_INV)
	{
		if(mcWaterCheck(o->oPosX, o->oPosZ) > o->oPosY)
		{
			for(i = 0; i < 40; i++)
				s_makeobj_nowpos(o, MODEL_WHITE_PARTICLE_SMALL, sm64::bhv::bhvBobombExplosionBubble());
		}
		else
			s_makeobj_nowpos(o, MODEL_SMOKE, sm64::bhv::bhvBobombBullyDeathSmoke());

		o->activeFlags = 0;
	}

	o->oOpacity -= 14;

	s_set_scale((f32)o->oTimer / 9.0f * FRAME_RATE_SCALER + 1.0);
}
