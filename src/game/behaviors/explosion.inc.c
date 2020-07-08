// explosion.c.inc

void bhv_explosion_init(void)
{
	create_sound_spawner(SOUND_GENERAL2_BOBOMB_EXPLOSION);
	set_environmental_camera_shake(SHAKE_ENV_EXPLOSION);

	o->oOpacity = 255;
}

void bhv_explosion_loop(void)
{
	s32 i;

	if(o->oTimer == 9 * FRAME_RATE_SCALER_INV)
	{
		if(find_water_level(o->oPosX, o->oPosZ) > o->oPosY)
		{
			for(i = 0; i < 40; i++)
				spawn_object(o, MODEL_WHITE_PARTICLE_SMALL, sm64::bhv::bhvBobombExplosionBubble());
		}
		else
			spawn_object(o, MODEL_SMOKE, sm64::bhv::bhvBobombBullyDeathSmoke());

		o->activeFlags = 0;
	}

	o->oOpacity -= 14;

	obj_scale((f32)o->oTimer / 9.0f * FRAME_RATE_SCALER + 1.0);
}
